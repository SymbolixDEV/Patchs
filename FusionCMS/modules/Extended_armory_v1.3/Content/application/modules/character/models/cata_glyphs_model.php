<?php

class cata_glyphs_model extends CI_Model
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
}