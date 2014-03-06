<?php

class Pingback extends MX_Controller
{
	private $ipsWhitelist = array(
		'174.36.92.186',
		'174.36.96.66',
		'174.36.92.187',
		'174.36.92.192',
		'174.37.14.28'
	);

	private $secret;
	private $key;
	private $params;
	private $signature;
	private $currency;
	private $uid;

	/**
	 * Load the config options
	 */
	public function __construct()
	{
		parent::__construct();

		$this->load->config('donate');
	}

	/**
	 * Handle the pingback request
	 */
	public function index()
	{
		$this->key = $this->config->item('paymentwall_key');
		$this->currency = $this->input->get('currency');
		$this->uid = $this->input->get('uid');

		$this->params = array(
			'uid' => $this->uid,
			'currency' => $this->currency,
			'type' => $this->input->get('type'),
			'ref' => $this->input->get('ref')
		);

		$this->secret = $this->config->item('paymentwall_secret_key');

		// Calculate the signature
		$this->signature = $this->calculatePingbackSignature();

		// Make sure it's really PaymentWall that is sending the data
		if(in_array($_SERVER['REMOTE_ADDR'], $this->ipsWhitelist)
		&& $this->input->get('sig') == $this->signature)
		{
			$this->process();
		}
		else
		{
			if(in_array($_SERVER['REMOTE_ADDR'], $this->ipsWhitelist))
			{
				die("WRONG SIGNATURE");
			}
			else
			{
				die("WRONG IP");
			}

			die("Permission denied");
		}
	}

	/**
	 * Proceed with the payment processing
	 */
	private function process()
	{
		// Update account with donation points
		$this->db->query("UPDATE `account_data` SET `dp` = `dp` + ? WHERE `id` = ?", array($this->currency, $this->uid));

		// Update the monthly income logs
		$this->updateMonthlyIncome(); 

		die("OK");
	}

	/**
	 * Calculate the signature value
	 * @return String
	 */
	private function calculatePingbackSignature()
	{
		$str = '';

		foreach($this->params as $k => $v)
		{
			$str .= "$k=$v";
		}

		$str .= $this->secret;

		return md5($str);
	}  

	/**
	 * Keep track of the monthly income
	 */
	private function updateMonthlyIncome()
	{
		$query = $this->db->query("SELECT COUNT(*) AS `total` FROM monthly_income WHERE month=?", array(date("Y-m")));

		$row = $query->result_array();

		if($row[0]['total'])
		{
			$this->db->query("UPDATE monthly_income SET amount = amount + ".$this->currency." WHERE month=?", array(date("Y-m")));
		}
		else
		{
			$this->db->query("INSERT INTO monthly_income(month, amount) VALUES(?, ?)", array(date("Y-m"), floor($this->currency)));
		}
	} 
}
