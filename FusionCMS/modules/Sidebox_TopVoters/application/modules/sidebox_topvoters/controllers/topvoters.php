<?php

class Topvoters extends MX_Controller implements Sidebox
{
	public function __construct()
	{
		parent::__construct();
		
		// Load our assets
		$this->load->config('sidebox_topvoters/topvoters_config');
		$this->load->model('sidebox_topvoters/topvoters_model');
	}

	public function view()
	{
		$cache = $this->cache->get("sidebox_topvoters");
		
		if ($cache !== false)
		{
			$out = $cache;
		}
		else
		{
			$data = array(
				'url'		=> $this->template->page_url,
				'module' 	=> 'sidebox_topvoters',
				'accounts'	=> $this->topvoters_model->getThisWeekAccounts($this->config->item("accounts_limit"))
			);
			
			$out = $this->template->loadPage("topvoters.tpl", $data);
			
			// Cache for 1 hour
			$this->cache->save("sidebox_topvoters", $out, strtotime($this->config->item("cache_time")));
		}
		
		return $out;
	}
}
