<?php

class Donate_wall extends MX_Controller
{
	/**
	 * Prevent unauthorized access and load the necessary helpers
	 */
	public function __construct()
	{
		parent::__construct();
		
		$this->user->is_logged_in();

		$this->load->helper('cookie');
		$this->load->config('donate');
	}
	
	/**
	 * Display the donation page
	 */
	public function index()
	{
		// Set the page title
		$this->template->setTitle("Donate");
		
		// Gather the data
		$data = array(
			"key" => $this->config->item('paymentwall_key'),
			"user_id" => $this->user->getId()
		);

		// Load the page and display it in a nice content box
		$output = $this->template->loadPage("donate.tpl", $data);
		$this->template->box("<span style='cursor:pointer;' onClick='window.location=\"".$this->template->page_url."ucp\"'>UCP</span> &rarr; Donate panel", $output, true);
	}
}
