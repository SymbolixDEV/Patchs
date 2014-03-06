<?php

class Character_transfer extends MX_Controller
{
	private $js;
	private $css;
	
	private $language;
	
	const MODULE_NAME			= 'character_transfer';
	const MODULE_PATH 			= 'modules/character_transfer';
	
	function __construct()
	{
		parent::__construct();
		
		$this->user->userArea();
		
		// Set JS and CSS paths
		$this->js 	= self::MODULE_PATH . "/js/character_transfer.js";
		$this->css 	= self::MODULE_PATH . "/css/character_transfer.css";
		
		$this->load->model("character_transfer_model");
		$this->load->config(self::MODULE_NAME . '/character_transfer_config');
		
		// Format the language strings
		$this->language = $this->config->item('cta_language');
		$this->FormatLanguage();
		
		if ($this->user->getAccountStatus() != 'Active')
		{
			$this->BannedPage();
			die;
		}
	}
	
	private function FormatLanguage()
	{
		$PriceCurrency = ($this->config->item('cta_price_currency') == 'dp') ? $this->language['DP'] : $this->language['VP'];
		$this->language['COST_EXPL'] = str_replace(array('[PRICE]', '[CURRENCY]'), array($this->config->item('cta_price'), $PriceCurrency), $this->language['COST_EXPL']);
		
		$this->language['ERROR_CHAR_LIMIT'] = str_replace('[LIMIT]', $this->config->item('cta_characters_limit'), $this->language['ERROR_CHAR_LIMIT']);
	}
	
	/**
	 * Initialize
	 */
	public function index()
	{
		requirePermission("use_cta");
		
		$this->template->setTitle($this->language['TITLE']);
		
		// Prepare the compitable realms
		$CompitableRealms = array();
		foreach ($this->realms->getRealms() as $realmData)
		{
			$CompitableRealms[] = array('id' => $realmData->getId(), 'name' => $realmData->getName(), 'characters' => $this->GetMyCharacters($realmData->getId()));
		}
		unset($realmData);
		
		if (empty($CompitableRealms))
		{
			$this->ErrorPage();
			return;
		}
		
		// Get the first compitable realm
		$FirstRealm = $CompitableRealms[0];
		
		// Set the page data
		$pageData = array(
			"url" 				=> $this->template->page_url,
			"first_realm"		=> $FirstRealm,
			"realms" 			=> $CompitableRealms,
			"cta_lang"			=> $this->language,
			"price"				=> $this->config->item('cta_price'),
			"price_currency"	=> $this->config->item('cta_price_currency')
		);
		
		unset($CompitableRealms, $FirstRealm);
		
		$pageTpl = $this->template->loadPage("character_transfer.tpl", $pageData);

		$data = array(
			"module" => "default", 
			"headline" => $this->language['TITLE'],
			"content" => $pageTpl
		);

		$page = $this->template->loadPage("page.tpl", $data);
		
		$keywords = $this->language['KEYWORDS'];
		$description = $this->language['DESCRIPTION'];
		
		$this->template->setDescription($description);
		$this->template->setKeywords($keywords);

		$this->template->view($page, $this->css, $this->js);
	}
	
	public function success($realmId = 1, $character = 'unknown', $account = 'unknown')
	{
		requirePermission("use_cta");
		
		// format the variables
		$character = preg_replace("/[^a-zA-z0-9_-]/", "", ucfirst(strtolower($character)));
		$account = preg_replace("/[^a-zA-z0-9_-]/", "", ucfirst(strtolower($account)));
		$realm = $this->realms->getRealm($realmId)->getName();
		
		$this->template->setTitle($this->language['TITLE']);
		
		// Fomat the success language message
		$this->language['SUCCESS_MSG'] = str_replace(array('[CHARACTER]', '[REALM_NAME]', '[ACCOUNT]'), array($character, $realm, $account), $this->language['SUCCESS_MSG']);
		
		// Set the page data
		$pageData = array(
			"url" 				=> $this->template->page_url,
			"realm"				=> $realm,
			"character"			=> $character,
			"account"			=> $account,
			"cta_lang"			=> $this->language
		);
		
		$pageTpl = $this->template->loadPage("character_transfer_success.tpl", $pageData);

		$data = array(
			"module" => "default", 
			"headline" => $this->language['TITLE'],
			"content" => $pageTpl
		);

		$page = $this->template->loadPage("page.tpl", $data);
		
		$keywords = $this->language['KEYWORDS'];
		$description = 'The character ' . $character . ' from realm '.$realm.' has been successfully transferred to account '.$account.'.';
		
		$this->template->setDescription($description);
		$this->template->setKeywords($keywords);

		$this->template->view($page, $this->css, false);
	}
	
	private function ErrorPage()
	{
		$this->template->setTitle("An error occured!");

		$data = array(
			"module" => "default", 
			"headline" => 'An error occured!', 
			"content" => "<center style='margin:10px;font-weight:bold;'>This module cannot work with no realms.</center>"
		);

		$page = $this->template->loadPage("page.tpl", $data);

		$this->template->view($page);
	}
	
	private function BannedPage()
	{
		$this->template->setTitle("An error occured!");

		$data = array(
			"module" => "default", 
			"headline" => 'An error occured!', 
			"content" => "<center style='margin:10px;font-weight:bold;'>" . (isset($this->language['BANNED_MSG']) ? $this->language['BANNED_MSG'] : 'Your account is banned!') . "</center>"
		);

		$page = $this->template->loadPage("page.tpl", $data);

		$this->template->view($page);
	}
	
	public function process()
	{
		requirePermission("use_cta");
		
		$this->load->helper(array('form', 'url'));
		$this->load->library('form_validation');
		
		// Get the post variables
		$RealmId = (int)$this->input->post('realmId');
		$Character = $this->input->post('character');
		$destination = $this->input->post('destination');
		$CharactersLimit = (int)$this->config->item('cta_characters_limit');
		$Price = (int)$this->config->item('cta_price');
		$PriceCurrency = $this->config->item('cta_price_currency');
		
		//Load the form validations for if they tried to sneaky bypass our js system
		$this->form_validation->set_rules('destination', 'destination account', 'trim|required|min_length[3]|max_length[14]|xss_clean|alpha_numeric');
		
		// Validate the destination account
		if ($this->form_validation->run() == false)
		{
			die(form_error('destination'));
		}
		
		// Get the realm object
		if (!($realmObj = $this->realms->getRealm($RealmId)))
		{
			die($this->language['ERROR_REALM']);
		}
		
		// Make sure the character exists and get the GUID
		if (!($GUID = $realmObj->getCharacters()->getGuidByName($Character)))
		{
			die($this->language['ERROR_CHARACTER']);
		}
		
		// Make sure the character belongs to this account
		if (!$realmObj->getCharacters()->characterBelongsToAccount($GUID, $this->user->getId()))
		{
			die($this->language['ERROR_BELONGS']);
		}
		
		// Check if the character is online
		if ($realmObj->getCharacters()->isOnline($GUID))
		{
			die($this->language['ERROR_ONLINE']);
		}
		
		// Validate the destination account
		if (!($DestAccID = $this->external_account_model->getId($destination)))
		{
			die($this->language['ERROR_DEST_ACC']);
		}
		
		// Check if the destination is our acc (prevent user mistakes)
		if ((int)$DestAccID == (int)$this->user->getId())
		{
			die($this->language['ERROR_DEST_ME']);
		}
		
		// Make sure the destination account have a free slto for character
		if ($realmObj->getCharacters()->getCharacterCount($DestAccID) >= $CharactersLimit)
		{
			die($this->language['ERROR_CHAR_LIMIT']);
		}
		
		// Check if the user has enough currency
		// If the service is not free
		if ($Price != 0)
		{
			if ($PriceCurrency == 'dp')
			{
				if ($this->user->getDp() < $Price)
				{
					die($this->language['ERROR_PRICE_DP']);
				}
			}
			else
			{
				if ($this->user->getVp() < $Price)
				{
					die($this->language['ERROR_PRICE_VP']);
				}
			}
		}
		
		// Process the transfer
		// Take away the price if the update is successful
		if ($this->character_transfer_model->setCharacterAccount($GUID, $DestAccID, $RealmId))
		{
			// Update the user currency
			if ($Price > 0)
			{
				if ($PriceCurrency == 'dp')
				{
					$StartCurrency = $this->user->getDp();
					$this->user->setDp($StartCurrency - $Price);
					$EndCurrency = $this->user->getDp();
				}
				else
				{
					$StartCurrency = $this->user->getVp();
					$this->user->setVp($StartCurrency - $Price);
					$EndCurrency = $this->user->getVp();
				}
			}
			
			// Log this transfer
			$this->character_transfer_model->InsertLog($GUID, ucfirst(strtolower($Character)), $DestAccID, ucfirst(strtolower($destination)), $RealmId, $Price, $PriceCurrency, $StartCurrency, $EndCurrency);
			
			// Return Success
			die('OK');
		}
		else
		{
			// failure
			die($this->language['ERROR_WEB_FAIL']);
		}
	}
	
	private function GetMyCharacters($realmId)
	{
		if ($this->user->isOnline())
		{
			$CharObj = $this->realms->getRealm($realmId)->getCharacters();
			
			//Open the connection to the databases
			$CharObj->connect();
			
			// Get our characters in this realm
			return $CharObj->getCharactersByAccount($this->user->getId());
		}
		
		return false;
	}
}
