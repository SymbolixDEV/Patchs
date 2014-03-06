<?php

class wotlk_dbc_model extends CI_Model
{
	private $runtimeCache = array();
	
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
	
	public function getGlyphInfo($id)
	{
		$query = $this->db->query("	SELECT `data_wotlk_glyphproperties`.`id`, `data_wotlk_glyphproperties`.`spellid`, `data_wotlk_glyphproperties`.`typeflags`, `data_wotlk_spell`.`spellname` AS name
									FROM `data_wotlk_glyphproperties` 
									INNER JOIN `data_wotlk_spell` on `data_wotlk_glyphproperties`.`spellid` = `data_wotlk_spell`.`spellID`  
									WHERE `data_wotlk_glyphproperties`.`id` = ? 
									LIMIT 1;", array($id));
		
		if($query->num_rows() > 0)
		{
			$result = $query->result_array();
			return $result[0];
		}
		
		unset($query);
		
		return false;
	}
	
	public function getEnchantmentInfo($id)
	{
		if (isset($this->runtimeCache[$id]))
		{
			return $this->runtimeCache[$id][0];
		}
		
		$query = $this->db->query("	SELECT 
										`data_wotlk_spellitemenchantment`.`id`, 
										`data_wotlk_spellitemenchantment`.`description`, 
										`data_wotlk_spellitemenchantment`.`GemID`, 
										`data_wotlk_spellitemenchantment`.`EnchantmentCondition`, 
										`data_wotlk_gemproperties`.`color` 
									FROM `data_wotlk_spellitemenchantment` 
									LEFT JOIN `data_wotlk_gemproperties` ON `data_wotlk_gemproperties`.`SpellItemEnchantement` = `data_wotlk_spellitemenchantment`.`id` 
									WHERE `data_wotlk_spellitemenchantment`.`id` = ? 
									LIMIT 1;", array($id));
									
		if ($query->num_rows() > 0)
		{
			$result = $query->result_array();
			
			//save to cache
			$this->runtimeCache[$id] = $result;
			
			return $result[0];
		}
		
		unset($query);
		
		return false;
	}
	
	public function getEnchantmentConditions($ConditionEntry)
	{
		$query = $this->db->query("	SELECT *
									FROM `data_wotlk_spellitemenchantmentcondition` 
									WHERE `id` = ? 
									LIMIT 1;", array($ConditionEntry));
									
		if ($query->num_rows() > 0)
		{
			$result = $query->result_array();
			
			return $result[0];
		}
		
		unset($query);
		
		return false;
	}
	
	public function getAchievementInfo($id)
	{
		$query = $this->db->query("	SELECT 
										`data_wotlk_achievement`.`id`, 
										`data_wotlk_achievement`.`name`, 
										`data_wotlk_achievement`.`description`, 
										`data_wotlk_achievement`.`points`, 
										`data_wotlk_achievement`.`icon`,  
										`data_wotlk_spellicons`.`iconname` 
									FROM `data_wotlk_achievement` 
									LEFT JOIN `data_wotlk_spellicons` ON `data_wotlk_spellicons`.`id` = `data_wotlk_achievement`.`icon` 
									WHERE `data_wotlk_achievement`.`id` = ? 
									LIMIT 1;", array($id));
		
		if($query->num_rows() > 0)
		{
			$result = $query->result_array();
			return $result[0];
		}
		
		unset($query);
		
		return false;
	}
}