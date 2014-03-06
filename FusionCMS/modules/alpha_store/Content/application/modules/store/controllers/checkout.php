<?php

class Checkout extends MX_Controller
{
	private $vp;
	private $dp;
	private $count;

	public function __construct()
	{
		parent::__construct();
		
		$this->user->userArea();

		$this->load->model("store_model");

		requirePermission("view");
	}

	/**
	 * Main method to serve the checkout action
	 */
	public function index()
	{
		$realm = $this->input->post('realm');
		$cart = $this->input->post("cart");
		
		// Make sure they sent us a cart object
		if (!$cart)
		{
			die("Please provide a cart object");
		}
		
		// Verify the realm
		if (!$realm || !$this->realms->realmExists($realm))
		{
			die("The selected realm does not exist or is invalid.");
		}
		
		try
		{
			// Decode the JSON object
			$cart = json_decode($cart, true);
		}
		catch(Exception $error)
		{
			die("Please provide a valid cart object");
		}

		// Make sure they don't submit an empty array
		if(count($cart) == 0)
		{
			die("Your cart can't be empty");
		}

		$items = array();
		$realms = array();

		// Load all items
		foreach($cart as $item)
		{
			// Load the item
			$items[$item['id']] = $this->store_model->getItem($item['id']);

			// Make sure the item exists
			if($items[$item['id']] != false)
			{
				$this->count++;

				// Keep track of how much it costs
				if($item['type'] == "vp" && !empty($items[$item['id']]['vp_price']))
				{
					$this->vp += $items[$item['id']]['vp_price'];
				}
				elseif($item['type'] == "dp" && !empty($items[$item['id']]['dp_price']))
				{
					$this->dp += $items[$item['id']]['dp_price'];
				}
				else
				{
					die(lang("free_items", "store"));
				}
			}
		
			// Put it in the realm array
			if(!isset($realms[$realm]))
			{
				$realms[$realm] = array(
					'name' => $this->realms->getRealm($realm)->getName(),
					'items' => array(),
					'characters' => $this->realms->getRealm($realm)->getCharacters()->getCharactersByAccount(),
				);
			}
			
			// Re-assing the realmid of the item
			$items[$item['id']]['realm'] = $realm;
			
			array_push($realms[$realm]['items'], $items[$item['id']]);
		}

		// Make sure the user can afford it
		if(!$this->canAfford())
		{		
			$output = $this->template->loadPage("checkout_error.tpl");

			die($output);
		}

		// Prepare the data
		$data = array(
			'realms' => $realms,
			'url' => $this->template->page_url,
			'vp' => $this->vp,
			'dp' => $this->dp,
			'count' => $this->count
		);

		// Load the checkout view
		$output = $this->template->loadPage("checkout.tpl", $data);

		// Output the content
		die($output);
	}

	/**
	 * Check if the user can afford what he's trying to buy
	 * @return Boolean
	 */
	private function canAfford()
	{
		if($this->vp > 0 && $this->vp > $this->user->getVp())
		{
			return false;
		}
		elseif($this->dp > 0 && $this->dp > $this->user->getDp())
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}