<?php

class Topvoters_model extends CI_Model
{
	private $db;

	public function __construct()
	{
		parent::__construct();

		$this->db = $this->load->database("cms", true);
	}
	
	public function getThisWeekAccounts($limit = 5)
	{
		$weekRange = $this->getWeekStartEnd();
		
		$query = $this->db->query("	SELECT 
										COUNT(`vote_log`.`user_id`) AS count, 
										`vote_log`.`user_id`, 
										`account_data`.`nickname` 
									FROM `vote_log` 
									LEFT JOIN `account_data` ON `account_data`.`id` = `vote_log`.`user_id` 
									WHERE `time` > ? 
									GROUP BY `vote_log`.`user_id` 
									ORDER BY count DESC 
									LIMIT ?;", 
								array(strtotime($weekRange[0]), $limit));
		
		if ($query->num_rows() > 0)
		{
			$array = $query->result_array();
			
			return $array;
		}
		else 
		{
			return false;	
		}
	}
	
	public function getWeekStartEnd()
	{
       	$ts = time();
		
		//check if today is monday
		if (date('w', $ts) == 1)
		{
			$start = strtotime('today', $ts);
		}
		else
		{
			$start = strtotime('this week last monday', $ts);
		}
		//Check if today is sunday
		if (date('w', $ts) == 0)
		{
			$end = strtotime('today', $ts);
		}
		else
		{
			$end = strtotime('this week next sunday', $ts);
		}
		
		//Add 23 hours and 59 minutes to the END date
		return array(date('Y-m-d H:i', $start), date('Y-m-d H:i', $end + 60*60*23 + 60*59));
	}
}