<?php

class wotlk_glyphs_model extends CI_Model
{
	public function __construct()
	{
		parent::__construct();
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
}