<?php

class cata_dbc_model extends CI_Model
{
	private $runtimeCache = array();
	private $build = '13633';
	
	public function __construct()
	{
		parent::__construct();
	}

	public function setBuild($build)
	{
		$this->build = $build;
	}
	
	private function getTablePrefix()
	{
		return 'data_cata_' . $this->build;
	}

	public function getTalentTabs($class)
	{
		$query = $this->db->query("	SELECT `".$this->getTablePrefix()."_talenttab`.`id`, `".$this->getTablePrefix()."_talenttab`.`name`, `".$this->getTablePrefix()."_talenttab`.`classes`, `".$this->getTablePrefix()."_talenttab`.`order`, `".$this->getTablePrefix()."_spellicons`.`iconname` AS icon 
									FROM `".$this->getTablePrefix()."_talenttab` 
									INNER JOIN `".$this->getTablePrefix()."_spellicons` ON `".$this->getTablePrefix()."_talenttab`.`spellicon` = `".$this->getTablePrefix()."_spellicons`.`id`
									WHERE `".$this->getTablePrefix()."_talenttab`.`classes` = ? 
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
		$this->db->select('*')->from($this->getTablePrefix().'_talent')->where(array('tab' => $tab))->order_by('id', 'ASC');
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
		$query = $this->db->query("SELECT `iconname` AS icon FROM `".$this->getTablePrefix()."_spellicons` WHERE `id` = (SELECT `".$this->getTablePrefix()."_spell`.`SpellIconID` FROM `".$this->getTablePrefix()."_spell` WHERE `".$this->getTablePrefix()."_spell`.`spellID` = ?) LIMIT 1;", array($spell));
		
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
		$query = $this->db->query("	SELECT `".$this->getTablePrefix()."_glyphproperties`.`id`, `".$this->getTablePrefix()."_glyphproperties`.`spellid`, `".$this->getTablePrefix()."_glyphproperties`.`typeflags`, `".$this->getTablePrefix()."_spell`.`SpellName` AS name
									FROM `".$this->getTablePrefix()."_glyphproperties` 
									INNER JOIN `".$this->getTablePrefix()."_spell` on `".$this->getTablePrefix()."_glyphproperties`.`spellid` = `".$this->getTablePrefix()."_spell`.`spellID`  
									WHERE `".$this->getTablePrefix()."_glyphproperties`.`id` = ? 
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
										`".$this->getTablePrefix()."_spellitemenchantment`.`id`, 
										`".$this->getTablePrefix()."_spellitemenchantment`.`description`, 
										`".$this->getTablePrefix()."_spellitemenchantment`.`GemID`, 
										`".$this->getTablePrefix()."_spellitemenchantment`.`EnchantmentCondition`, 
										`".$this->getTablePrefix()."_gemproperties`.`color` 
									FROM `".$this->getTablePrefix()."_spellitemenchantment` 
									LEFT JOIN `".$this->getTablePrefix()."_gemproperties` ON `".$this->getTablePrefix()."_gemproperties`.`SpellItemEnchantement` = `".$this->getTablePrefix()."_spellitemenchantment`.`id` 
									WHERE `".$this->getTablePrefix()."_spellitemenchantment`.`id` = ? 
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
									FROM `".$this->getTablePrefix()."_spellitemenchantmentcondition` 
									WHERE `id` = ? 
									LIMIT 1;", array($ConditionEntry));
		
		if($this->db->_error_message())
		{
			die($this->db->_error_message());
		}
				
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
										`".$this->getTablePrefix()."_achievement`.`id`, 
										`".$this->getTablePrefix()."_achievement`.`name`, 
										`".$this->getTablePrefix()."_achievement`.`description`, 
										`".$this->getTablePrefix()."_achievement`.`points`, 
										`".$this->getTablePrefix()."_achievement`.`icon`,  
										`".$this->getTablePrefix()."_spellicons`.`iconname` 
									FROM `".$this->getTablePrefix()."_achievement` 
									LEFT JOIN `".$this->getTablePrefix()."_spellicons` ON `".$this->getTablePrefix()."_spellicons`.`id` = `".$this->getTablePrefix()."_achievement`.`icon` 
									WHERE `".$this->getTablePrefix()."_achievement`.`id` = ? 
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