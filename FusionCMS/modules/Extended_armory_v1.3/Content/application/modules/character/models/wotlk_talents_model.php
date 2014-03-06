<?php

class wotlk_talents_model extends CI_Model
{
	public function __construct()
	{
		parent::__construct();
	}

	public function getTalentTabs($class)
	{
		$query = $this->db->query("	SELECT `data_wotlk_talenttab`.`id`, `data_wotlk_talenttab`.`name`, `data_wotlk_talenttab`.`classes`, `data_wotlk_talenttab`.`order`, `data_wotlk_spellicons`.`iconname` AS icon 
									FROM `data_wotlk_talenttab` 
									INNER JOIN `data_wotlk_spellicons` ON `data_wotlk_talenttab`.`spellicon` = `data_wotlk_spellicons`.`id`
									WHERE `data_wotlk_talenttab`.`classes` = ? 
									ORDER BY `order` ASC;", 
									array($class));
		
		if($query && $query->num_rows() > 0)
		{
			$result = $query->result_array();
			return $result;
		}
		
		unset($query);
		
		return false;
	}
	
	public function getTalentsForTab($tab)
	{
		$this->db->select('*')->from('data_wotlk_talent')->where(array('tab' => $tab))->order_by('id', 'ASC');
		$query = $this->db->get();
		
		if($query->num_rows() > 0)
		{
			$result = $query->result_array();
			return $result;
		}
		
		unset($query);
		
		return false;
	}
	
	public function getSpellIcon($spell)
	{
		$query = $this->db->query("SELECT `iconname` AS icon FROM `data_wotlk_spellicons` WHERE `id` = (SELECT `data_wotlk_spell`.`spellicon` FROM `data_wotlk_spell` WHERE `data_wotlk_spell`.`spellID` = ?) LIMIT 1;", array($spell));
		
		if($query && $query->num_rows() > 0)
		{
			$result = $query->result_array();
			return $result[0];
		}
		
		unset($query);
		
		return false;
	}
}