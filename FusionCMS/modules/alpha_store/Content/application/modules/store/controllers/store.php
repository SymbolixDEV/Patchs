<?php

class Store extends MX_Controller
{
	private $realmId;
	private $search;
	private $sort;
	private $quality;
	private $page;
	private $PerPage = 10;
	
	const STORE_SORT_DEFAULT		= 0;
	const STORE_SORT_NAME			= 1;
	const STORE_SORT_PRICE_VP		= 2;
	const STORE_SORT_PRICE_DP		= 3;
	const STORE_SORT_QUALITY		= 4;
	
	const STORE_MIN_QUALITY = -1;
	const STORE_MAX_QUALITY = 7;
	
	public function __construct()
	{
		parent::__construct();
		
		$this->output->set_header("Cache-Control: no-store, no-cache, must-revalidate");
		$this->output->set_header("Cache-Control: post-check=0, pre-check=0");
		$this->output->set_header("Pragma: no-cache");

		$this->user->userArea();

		$this->load->model("store_model");
		$this->load->library('pagination');
		
		$this->load->config('store');
		$this->load->config('translation');
		
		// Override per page
		$this->PerPage = $this->config->item("items_per_page");
		
		requirePermission("view");
	}
	
	/**
	 * Determinate which Id to assign
	 */
	private function setRealm($realmId)
	{
		// If no realm is specified and we have a cookie
		if ($realmId === false && $this->getSelectedRealm() !== false)
		{
			$realmId = $this->getSelectedRealm();
		}
		
		// Get the first realm if we have no realm
		if ($realmId === false || !$this->realms->realmExists($realmId))
		{
			$realms = $this->realms->getRealms();
			
			if ($realms)
			{
				foreach ($realms as $realmData)
				{
					$realmId = $realmData->getId();
					break;
				}
				unset($realmData);
			}
			unset($realms);
		}
		
		// Check if valid X-Y format
		if ($realmId !== false && is_numeric($realmId) && $this->realms->realmExists($realmId))
		{
			$this->realmId = $realmId;
		}
		else
		{
			$this->getError();
			die;
		}
	}
	
	private function setSearch($str)
	{
		if ($str == '' || $str == 'null')
		{
			$this->search = false;
			return;
		}
		
		// Decode the string
		$str = rawurldecode($str);
		
		$str = preg_replace("/[^A-Za-z0-9-' ]/i", "", $str); 
		
		// Check the length
		if (strlen($str) < 3)
		{
			$this->search = false;
			return;
		}
		
		$this->search = $str;
	}
	
	private function setSort($id)
	{
		if (is_numeric($id) && (int)$id >= self::STORE_SORT_DEFAULT && (int)$id <= self::STORE_SORT_QUALITY)
		{
			$this->sort = (int)$id;
		}
		else
		{
			$this->sort = self::STORE_SORT_DEFAULT;
		}
	}
	
	private function setQuality($id)
	{
		if (is_numeric($id) && (int)$id >= self::STORE_MIN_QUALITY && (int)$id <= self::STORE_MAX_QUALITY)
		{
			$this->quality = (int)$id;
		}
		else
		{
			$this->quality = self::STORE_MIN_QUALITY;
		}
	}
	
	private function setPage($page)
	{
		if (is_numeric($page))
		{
			$this->page = (int)$page;
		}
		else
		{
			$this->page = 0;
		}
	}
	
	public function index($realm = false, $sort = self::STORE_SORT_DEFAULT, $quality = self::STORE_MIN_QUALITY, $search = false, $page = 0)
	{
		requirePermission("view");
		
		$this->setRealm($realm);
		$this->setSort($sort);
		$this->setQuality($quality);
		$this->setSearch($search);
		$this->setPage($page);
		
		clientLang("cant_afford", "store");
		clientLang("hide", "store");
		clientLang("show", "store");
		clientLang("loading", "store");
		clientLang("want_to_buy", "store");
		clientLang("yes", "store");
		clientLang("checkout", "store");
		clientLang("vp", "store");
		clientLang("dp", "store");
		
		// Prepare the compitable realms
		$CompitableRealms = array();
		foreach ($this->realms->getRealms() as $realmData)
		{
			$CompitableRealms[] = array('id' => $realmData->getId(), 'name' => $realmData->getName());
		}
		unset($realmData);
		
		// Setup some of the translation strings in array
		$Translation = array(
			'realm'	=> $this->config->item('lang_realm'),
			'no_items' => $this->config->item('lang_no_items'),
			'quality'  => $this->config->item('lang_quality')
		);
		
		// Get the cart save
		if (isset($_SESSION['store_cart']) && is_array($_SESSION['store_cart']) && (count($_SESSION['store_cart']) > 0))
		{
			$cart = $_SESSION['store_cart'];
			
			$index = 0;
			foreach ($cart as $key => $citem)
			{
				$itemInfo = $this->store_model->getItem($citem['id']);
				// save some stuff
				$cart[$key]['index'] = $index;
				$cart[$key]['info'] = $itemInfo;
				$cart[$key]['realm'] = ($itemInfo['realm'] == '-1' ? 'All Realms' : $this->realms->getRealm($itemInfo['realm'])->getName());
				$index++;
			}
			unset($itemInfo, $citem, $key);
		}
		else
		{
			$cart = false;
		}
		
		// Gather the template data
		$data = array(
			'url' => $this->template->page_url,
			'image_path' => $this->template->image_path,
			'vp' => $this->user->getVp(),
			'dp' => $this->user->getDp(),
			'data' => $this->getData(),
			"selected_realm" => $this->realmId,
			"realms" => $CompitableRealms,
			"translation" => $Translation,
			"filters" => array(
				'sort' => $this->sort, 
				'quality' => $this->quality, 
				'search' => $this->search, 
				'search_encoded' => ($this->search) ? rawurlencode($this->search) : false, 
				'page' => $this->page
			),
			"cart" => $cart
		);
		
		unset($CompitableRealms);
		
		// Load the content
		$content = $this->template->loadPage("store.tpl", $data);

		// Put the content in a box
		$page = $this->template->box("<span style='cursor:pointer;' onClick='window.location=\"".$this->template->page_url."ucp\"'>".lang("ucp")."</span> &rarr; ".lang("item_store", "store"), $content);

		// Output the content
		$this->template->view($page, "modules/store/css/store.css", "modules/store/js/store.js");
	}
	
	/**
	 * Show "character doesn't exist" error
	 */
	private function getError($get = false)
	{
		$this->template->setTitle($this->config->item('lang_error_title'));

		$data = array(
			"module" => "default", 
			"headline" => $this->config->item('lang_error_headline'), 
			"content" => "<center style='margin:10px;font-weight:bold;'>".$this->config->item('lang_error_text')."</center>"
		);

		$page = $this->template->loadPage("page.tpl", $data);

		if ($get)
		{
			return $page;
		}
		else
		{
			$this->template->view($page);
		}
	}
	
	private function getSelectedRealm()
	{
		if (isset($_COOKIE['fcms_store_realm']) && (int)$_COOKIE['fcms_store_realm'] > 0)
		{
			return (int)$_COOKIE['fcms_store_realm'];
		}
		
		return false;
	}

	/**
	 * Get all realms, item groups and items and format them nicely in an array
	 * @return Array
	 */
	private function getData()
	{
		$data = array();

		// Get the Auctions count and Auctions for the current page
		$TotalItems = $this->store_model->getItemsCount($this->realmId, $this->sort, $this->quality, $this->search);
		$ItemsLimit = $this->page . ',' . $this->PerPage;
		// Load all items that belongs to this realm
		$data['items'] = $this->store_model->getItems($this->realmId, $this->sort, $this->quality, $this->search, $ItemsLimit);
		
		// Generate pagination
		$PaginationConfig['base_url'] = $this->template->page_url . 'store/index/' . $this->realmId . '/' . $this->sort . '/' . $this->quality . '/' . ($this->search ? $this->search : 'null') . '/';
		$PaginationConfig['total_rows'] = $TotalItems;
		$PaginationConfig['per_page'] = $this->PerPage; 
		$PaginationConfig['uri_segment'] = 7;
    	$PaginationConfig["num_links"] = 5;
		$PaginationConfig['next_link'] = $this->config->item('lang_pagi_next');
		$PaginationConfig['prev_link'] = $this->config->item('lang_pagi_prev');
		$PaginationConfig['last_link'] = $this->config->item('lang_pagi_last');
		$PaginationConfig['first_link'] = $this->config->item('lang_pagi_first');
		$PaginationConfig['anchor_class'] = 'direct="0" ';
		
		$this->pagination->initialize($PaginationConfig); 
		$data['pagination'] = $this->pagination->create_links();
		
		// Free some mem
		unset($PaginationConfig, $ItemsLimit, $TotalItems);
		
		// Add realm names
		if ($data['items'] && count($data['items']) > 0)
		{
			foreach ($data['items'] as $i => $item)
			{
				if ($item['realm'] == -1)
				{
					$data['items'][$i]['realm_name'] = $this->config->item('lang_all_realms');
				}
				else
				{
					$data['items'][$i]['realm_name'] = $this->realms->getRealm($item['realm'])->getName();
				}
			}
		}
		
		return $data;
	}

	/**
	 * Put items in their groups and put un-assigned items in a separate list
	 * @param Array $items
	 * @return Array
	 */
	private function formatItems($items)
	{
		if($items != false)
		{
			$data = array(
				'groups' => array(), // Holds group titles and their items
				'items' => array() // Holds items without a group
			);

			$currentGroup = null;

			// Loop through all items
			foreach($items as $item)
			{
				if(empty($item['group']))
				{
					array_push($data['items'], $item);
				}
				else
				{
					if($currentGroup != $item['group'])
					{
						$currentGroup = $item['group'];

						// Assign the name to a group
						$data['groups'][$item['group']]['title'] = $this->store_model->getGroupTitle($item['group']);

						// Create the item array
						$data['groups'][$item['group']]['items'] = array();
					}

					array_push($data['groups'][$item['group']]['items'], $item);
				}
			}

			return $data;
		}
	}
	
	public function getCart()
	{
		die((isset($_SESSION['store_cart']) && is_array($_SESSION['store_cart']) && (count($_SESSION['store_cart']) > 0)) ? json_encode($_SESSION['store_cart']) : '0');
	}
	
	public function addToCart()
	{
		$id = $this->input->post('id');
		$price = $this->input->post('price');
		$priceType = $this->input->post('priceType');
		
		if (!$id || !$price || !$priceType)
		{
			die ('No parameters passed.');
		}
		
		// Prepare the associative key
		$key = $id . '-' . $priceType;
		
		// Check if the item is in the cart
		if (isset($_SESSION['store_cart'][$key]))
		{
			$_SESSION['store_cart'][$key]['count']++;
		}
		else
		{
			$_SESSION['store_cart'][$key] = array(
				'id' => $id,
				'price' => $price,
				'priceType' => $priceType,
				'count' => 1
			);
		}
		die;
	}
	
	public function removeFromCart()
	{
		$id = $this->input->post('id');
		$priceType = $this->input->post('priceType');
		
		if (!$id || !$priceType)
		{
			die ('No parameters passed.');
		}
		
		// Prepare the associative key
		$key = $id . '-' . $priceType;
		
		// Check if the item is in the cart
		if (isset($_SESSION['store_cart'][$key]))
		{
			unset($_SESSION['store_cart'][$key]);
		}
		die;
	}
}