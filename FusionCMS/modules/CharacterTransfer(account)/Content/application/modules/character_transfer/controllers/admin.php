<?php

class Admin extends MX_Controller
{
	private $PerPage 			= 10;
	
	const MODULE_NAME			= 'character_transfer';
	const MODULE_PATH 			= 'modules/character_transfer';
	
	public function __construct()
	{
		// Make sure to load the administrator library!
		$this->load->library('administrator');
		$this->load->library('pagination');
		$this->load->model("character_transfer_model");
		
		parent::__construct();

		requirePermission("viewAdmin");
	}

	public function index($page = 0)
	{
		// Change the title
		$this->administrator->setTitle("Character transfer (Account) logs");
		
		// Get the Auctions count and Auctions for the current page
		$TotalLogs 	= $this->character_transfer_model->GetLogsCount();
		$Limit 		= $page . ',' . $this->PerPage;
		$Logs 		= $this->character_transfer_model->GetLogs($Limit);
		
		// Generate pagination
		$PaginationConfig['base_url'] = $this->template->page_url . self::MODULE_NAME . '/admin/index/';
		$PaginationConfig['total_rows'] = $TotalLogs;
		$PaginationConfig['per_page'] = $this->PerPage; 
		$PaginationConfig['uri_segment'] = 4;
    	$PaginationConfig["num_links"] = 5;

		$this->pagination->initialize($PaginationConfig); 
		$Pagination = $this->pagination->create_links();
		
		// Free some mem
		unset($PaginationConfig, $Limit, $TotalLogs);
		
		if ($Logs)
		{
			foreach ($Logs as $i => $log)
			{
				$Logs[$i]['realmName'] = $this->realms->getRealm($log['realm_id'])->getName();
			}
		}
		
		// Prepare my data
		$data = array(
			'url' 			=> $this->template->page_url,
			'logs'			=> $Logs,
			'pagination'	=> $Pagination
		);

		// Load my view
		$output = $this->template->loadPage("admin.tpl", $data);

		// Put my view in the main box with a headline
		$content = $this->administrator->box('Character transfer logs', $output);

		// Output my content. The method accepts the same arguments as template->view
		$this->administrator->view($content, self::MODULE_PATH . "/css/admin.css", self::MODULE_PATH . "/js/admin.js");
	}

	public function search()
	{
		$string = $this->input->post('string');
		
		if (!$string)
		{
			die();
		}
		else
		{
			$results = $this->character_transfer_model->SearchLogs($string);
			
			if ($results)
			{
				foreach ($results as $i => $log)
				{
					$results[$i]['realmName'] = $this->realms->getRealm($log['realm_id'])->getName();
				}
			}
			
			if(!$results)
			{
				die("<span>No matches</span>");
			}
			
			$data = array(
				'url' => $this->template->page_url,
				'results' => $results
			);

			$output = $this->template->loadPage("admin_list.tpl", $data);

			die($output);
		}
	}
}