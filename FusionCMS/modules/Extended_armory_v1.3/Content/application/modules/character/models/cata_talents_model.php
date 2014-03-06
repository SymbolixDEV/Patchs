<?php

class cata_talents_model extends CI_Model
{
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
}