<?php

class Character_transfer_model extends CI_Model
{
	public function setCharacterAccount($guid, $account, $realmId)
	{
		$db = $this->realms->getRealm($realmId)->getCharacters()->getConnection();
		
		$Table = table("characters", $realmId);
		$ColumnAccount = column("characters", "account", false, $realmId);
		$ColumnGUID = column("characters", "guid", false, $realmId);
		
		$query = $db->query("UPDATE ".$Table." SET ".$ColumnAccount." = ? WHERE ".$ColumnGUID." = ? LIMIT 1;", array($account, $guid));
		
		if ($query)
		{
			return true;
		}
		
		return false;
	}
	
	public function InsertLog($character, $character_name, $DestAccID, $DestAccName, $RealmId, $Price, $PriceCurrency, $StartCurrency, $EndCurrency)
	{
		$query = $this->db->query("	INSERT INTO `cta_logs` 
									(`account`, `account_name`, `character`, `character_name`, `dest_account`, `dest_account_name`, `realm_id`, `price`, `currency`, `start_currency`, `end_currency`, `time`) 
									VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);", 
									array($this->user->getId(), ucfirst(strtolower($this->user->getNickname())), $character, $character_name, $DestAccID, $DestAccName, $RealmId, $Price, $PriceCurrency, $StartCurrency, $EndCurrency, time()));
		
		if ($query)
		{
			return true;
		}
		
		return false;
	}
	
	public function GetLogsCount()
	{
		$query = $this->db->query("SELECT COUNT(*) AS total FROM `cta_logs`;");
		
		if ($query && $query->num_rows() > 0)
		{
			$results = $query->result_array();
			
			return (int)$results[0]['total'];
		}
		
		return 0;
	}
	
	public function GetLogs($Limit)
	{
		$query = $this->db->query("SELECT * FROM `cta_logs` ORDER BY `time` DESC LIMIT ".$Limit.";");
		
		if ($query && $query->num_rows() > 0)
		{
			return $query->result_array();
		}
		
		return false;
	}
	
	public function SearchLogs($string)
	{
		$query = $this->db->query("SELECT * FROM `cta_logs` WHERE `account_name` LIKE ? OR `character_name` LIKE ? OR `dest_account_name` LIKE ? ORDER BY `time` DESC;", array($string, $string, $string));
		
		if ($query && $query->num_rows() > 0)
		{
			return $query->result_array();
		}
		
		return false;
	}
}