<?php

class Cata_model extends CI_Model
{
	public $realm;
	private $build = '13633';
	private $storedEffects;
	private $debug = false;
	
	public function __construct()
	{
		parent::__construct();
	}
	
	public function setRealm($id)
	{
		$this->realm = $this->realms->getRealm($id);
	}
	
	public function setBuild($build)
	{
		$this->build = $build;
	}
	
	private function getTablePrefix()
	{
		return 'data_cata_' . $this->build;
	}
	
	public function enableDebugMode()
	{
		$this->debug = true;
	}
	
	public function Exists($spell)
	{
		$this->db->select('COUNT(*)')->from($this->getTablePrefix() . '_spell')->where(array('spellID' => $spell));
		$query = $this->db->get();
		
		if($query->num_rows() > 0)
			return true;
		
		unset($query);
		
		return false;
	}
	
	public function glyphExists($id)
	{
		$this->db->select('COUNT(*)')->from($this->getTablePrefix() . '_glyphproperties')->where(array('id' => $id));
		$query = $this->db->get();
		
		if($query->num_rows() > 0)
			return true;
		
		unset($query);
		
		return false;
	}
	
	public function getSpellInfo($spell)
	{
		$query = $this->db->query("SELECT `SpellName` AS name, `Rank` AS rank, `ToolTip` AS tooltip FROM `".$this->getTablePrefix()."_spell` WHERE `spellID` = ? LIMIT 1;", array($spell));
		
		if($query->num_rows() > 0)
		{
			$result = $query->result_array();
			
			if ($this->debug)
				var_dump($result[0]);
			
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
	
	private function getSpellDuration($id)
	{
		$query = $this->db->query("SELECT * FROM `".$this->getTablePrefix()."_spellduration` WHERE `durationID` = ? LIMIT 1;", array($id));
		
		if($query->num_rows() > 0)
		{
			$result = $query->result_array();
			
			return $result[0];
		}
		
		unset($query);
		
		return false;
	}
	
	private function getSpellRange($id)
	{
		$query = $this->db->query("SELECT `rangeMin`, `rangeMinFriendly`, `rangeMax`, `rangeMaxFriendly` FROM `".$this->getTablePrefix()."_spellrange` WHERE `rangeID` = ? LIMIT 1;", array($id));
		
		if($query->num_rows() > 0)
		{
			$result = $query->result_array();
			
			return $result[0];
		}
		
		unset($query);
		
		return false;
	}
	
	private function getSpellEffect($spell, $effectIndex)
	{
		//check if the effect is already stored
		if (isset($this->storedEffects[$spell][$effectIndex]))
			return $this->storedEffects[$spell][$effectIndex];
		
		$query = $this->db->query("SELECT * FROM `".$this->getTablePrefix()."_spelleffect` WHERE `EffectSpellId` = ? AND `EffectIndex` = ? LIMIT 1;", array($spell, $effectIndex));
		
		if($query->num_rows() > 0)
		{
			$result = $query->result_array();
			
			//store the effect record
			$this->storedEffects[$spell][$effectIndex] = $result[0];
			
			return $result[0];
		}
		
		unset($query);
		
		return false;
	}
	
	private function getSpellAuraOptions($optionsId)
	{
		$query = $this->db->query("SELECT * FROM `".$this->getTablePrefix()."_spellauraoptions` WHERE `Id` = ? LIMIT 1;", array($optionsId));
		
		if($query->num_rows() > 0)
		{
			$result = $query->result_array();
			
			return $result[0];
		}
		
		unset($query);
		
		return false;
	}
	
	private function getSpellTargetRestrictions($Id)
	{
		$query = $this->db->query("SELECT * FROM `".$this->getTablePrefix()."_spelltargetrestrictions` WHERE `Id` = ? LIMIT 1;", array($iId));
		
		if($query->num_rows() > 0)
		{
			$result = $query->result_array();
			
			return $result[0];
		}
		
		unset($query);
		
		return false;
	}
	
	private function selectRow($queryStr, $data)
	{
		$query = $this->db->query($queryStr, $data);
		
		if($query->num_rows() > 0)
		{
			$result = $query->result_array();
			
			return $result[0];
		}
		
		unset($query);
		
		return false;
	}
	
	public function getSpellDesc($spell)
	{
		$query = $this->db->query("
				SELECT `spellID`, `".$this->getTablePrefix()."_spellicons`.`iconname`, `SpellName`, `Rank`, `ToolTip` AS tooltip, `CastingTimeIndex`, `DurationIndex`, `powerType`, `rangeIndex`, `SchoolMask`, `runeCostID`, `SpellDifficultyId`, `SpellScalingId`, `SpellAuraOptionsId`, `SpellTargetRestrictionsId`
				FROM `".$this->getTablePrefix()."_spell` s, `".$this->getTablePrefix()."_spellicons`
				WHERE
					`spellID` = ?
					AND `".$this->getTablePrefix()."_spellicons`.`id` = `SpellIconID`
				LIMIT 1;
			",
			array($spell)
		);
		
		if($query->num_rows() > 0)
		{
			$result = $query->result_array();
			
			return $this->parseSpellDesc($result[0]);
		}
		
		unset($query);
		
		return false;	
	}
	
	private function parseSpellDesc($spellRow)
	{
		$lastduration = $this->getSpellDuration($spellRow['DurationIndex']);
		
		$signs = array('+', '-', '/', '*', '%', '^');
	
		$data = $spellRow['tooltip'];
		
		if ($this->debug)
			var_dump($data);
		
		//handle them if statements
		$data = $this->handleDescriptionIfStatements($data);
		
		//cosmetics
		$data = strtr($data, array("\r" => '', "\n" => '<br />'));
		$data = preg_replace('/\|cff([a-f0-9]{6})(.+?)\|r/i', '<span style="color: #$1;">$2</span>', $data);
		$data = preg_replace('/\|C00([a-f0-9]{6})(.+?)\|R/i', '<span style="color: #$1;">$2</span>', $data);
	
		$pos = 0;
		$str = '';
		while (false!==($npos=strpos($data, '$', $pos)))
		{
			if($npos!=$pos)
				$str .= substr($data, $pos, $npos-$pos);
			$pos = $npos+1;
	
			if('$' == substr($data, $pos, 1))
			{
				$str .= '$';
				$pos++;
				continue;
			}
	
			if(!preg_match('/^((([+\-\/*])(\d+);)?(\d*)(?:([lg].*?:.*?);|(\w\d*)))/', substr($data, $pos), $result))
				continue;
			
			if(empty($exprData[0]))
				$exprData[0] = 0;
	
			$op = $result[3];
			$oparg = $result[4];
			$lookup = $result[5];
			$var = $result[6] ? $result[6] : $result[7];
			$pos += strlen($result[0]);
	
			if(!$var)
				continue;
			
			$exprType = strtolower(substr($var, 0, 1));
			$exprData = explode(':', substr($var, 1));
			
			if ($this->debug)
			{
				echo 'Expression type';
				var_dump($exprType);
			}
			
			switch ($exprType)
			{
				case 'r':
					if(!isset($spellRow['rangeMax']))
						$spellRow = array_merge($spellRow, $this->getSpellRange($spellRow['rangeIndex']));
	
					$base = $spellRow['rangeMax'];
	
					if($op && is_numeric($oparg) && is_numeric($base))
					{
						$equation = $base.$op.$oparg;
						eval("\$base = $equation;");
					}
					$str .= $base;
					break;
				case 'z':
					$str .= htmlspecialchars('<Home>');
					break;
				case 'c':
					$exprData[0] = $exprData[0] - 1;
					
					if($lookup > 0 && isset($exprData[0]) && $exprData[0] !== false && $exprData[0] !== '')
						$effect = $this->getSpellEffect($lookup, $exprData[0]);
					else
						$effect = $this->getSpellEffect($spellRow['spellID'], $exprData[0]);
	
					$base = (int)$effect['EffectBasePoints'];
	
					if(in_array($op, $signs) && is_numeric($oparg) && is_numeric($base))
					{
						$equation = $base.$op.$oparg;
						eval("\$base = $equation;");
					}
	
					$str .= $base;
					$lastvalue = $base;
					break;
				case 's':
					$exprData[0] = $exprData[0] - 1;
					
					if($lookup > 0 && isset($exprData[0]) && $exprData[0] !== false && $exprData[0] !== '')
						$effect = $this->getSpellEffect($lookup, $exprData[0]);
					else
						$effect = $this->getSpellEffect($spellRow['spellID'], $exprData[0]);
					
					//is this right ?
					if (!$effect)
						$effect = $this->getSpellEffect(($lookup > 0 ? $lookup : $spellRow['spellID']), 0);
					
					//if(!$exprData[0]) $exprData[0] = 0; // has no role
					@$base = (int)$effect['EffectBasePoints'];
					
					if(in_array($op, $signs) && is_numeric($oparg) && is_numeric($base))
					{
						$equation = $base.$op.$oparg;
						eval("\$base = $equation;");
					}
					
					@$str .= round(abs($base).($effect['EffectDieSides'] > 1 ? ' to '.abs(($base+$effect['EffectDieSides'])) : ''));
					$lastvalue = $base;
					break;
				case 'o':
					$exprData[0] = $exprData[0] - 1;
					
					if($lookup > 0 && isset($exprData[0]) && $exprData[0] !== false && $exprData[0] !== '')
					{
						//Get the lookup effect
						$effect = $this->getSpellEffect($lookup, $exprData[0]);
						//Get the duration id
						$spell = $this->selectRow("SELECT `DurationIndex` FROM `".$this->getTablePrefix()."_spell` WHERE `spellID` = ? LIMIT 1;", array($lookup));
						//Get the duration entry
						$lastduration = $this->getSpellDuration($spell['DurationIndex']);
					}
					else
						$effect = $this->getSpellEffect($spellRow['spellID'], $exprData[0]);
					
					//if(!$exprData[0]) $exprData[0] = 0; //has no role
					$base = (int)$effect['EffectBasePoints'];
					
					$base = ((int)$lastduration['durationBase'] / (int)$effect['EffectAmplitude']) * abs($base);
					
					if(in_array($op, $signs) && is_numeric($oparg) && is_numeric($base))
					{
						$equation = $base.$op.$oparg;
						eval("\$base = $equation;");
					}
					
					$str .= (abs($base).($effect['EffectDieSides'] > 1 ? '-'.abs(($base+$effect['EffectDieSides'])) : ''));
					break;
				case 't':
					$exprData[0] = $exprData[0] - 1;
					
					if($lookup > 0 && isset($exprData[0]) && $exprData[0] !== false && $exprData[0] !== '')
						$effect = $this->getSpellEffect($lookup, $exprData[0]);
					else
						$effect = $this->getSpellEffect($spellRow['spellID'], $exprData[0]);
	
					//if(!$exprData[0]) $exprData[0] = 0; //has no role
					$base = (int)$effect['EffectAmplitude']/1000;
	
					// TODO!!
					if($base==0) $base = 1;
					// !!TODO
	
					if(in_array($op, $signs) && is_numeric($oparg) && is_numeric($base))
					{
						$equation = $base.$op.$oparg;
						eval("\$base = $equation;");
					}
					$str .= abs($base);
					$lastvalue = $base;
					break;
				case 'm':
					$exprData[0] = $exprData[0] - 1;
					
					if($lookup > 0 && isset($exprData[0]) && $exprData[0] !== false && $exprData[0] !== '')
						$effect = $this->getSpellEffect($lookup, $exprData[0]);
					else
						$effect = $this->getSpellEffect($spellRow['spellID'], $exprData[0]);
	
					// TODO!!
					//if(!$exprData[0]) $exprData[0] = 0; // has no role
	
					$base = (int)$effect['EffectBasePoints'];
					
					if(in_array($op, $signs) && is_numeric($oparg) && is_numeric($base))
					{
						$equation = $base.$op.$oparg;
						eval("\$base = $equation;");
					}
					$str .= abs($base);
					$lastvalue = $base;
					break;
				case 'x':
					$exprData[0] = $exprData[0] - 1;
					
					if($lookup > 0 && isset($exprData[0]) && $exprData[0] !== false && $exprData[0] !== '')
						$effect = $this->getSpellEffect($lookup, $exprData[0]);
					else
						$effect = $this->getSpellEffect($spellRow['spellID'], $exprData[0]);
	
					$base = $effect['EffectChainTarget'];
	
					if(in_array($op, $signs) && is_numeric($oparg) && is_numeric($base))
					{
						$equation = $base.$op.$oparg;
						eval("\$base = $equation;");
					}
					$str .= abs($base);
					$lastvalue = $base;
					break;
				case 'q':
					$exprData[0] = $exprData[0] - 1;
					
					if($lookup > 0 && isset($exprData[0]) && $exprData[0] !== false && $exprData[0] !== '')
						$effect = $this->getSpellEffect($lookup, $exprData[0]);
					else
						$effect = $this->getSpellEffect($spellRow['spellID'], $exprData[0]);
	
					//if(!($exprData[0])) $exprData[0] = 0; //has no role
					$base = $effect['EffectMiscValue'];
	
					if(in_array($op, $signs) && is_numeric($oparg) && is_numeric($base))
					{
						$equation = $base.$op.$oparg;
						eval("\$base = $equation;");
					}
					$str .= abs($base);
					$lastvalue = $base;
					break;
				case 'a':
					$exprData[0] = $exprData[0] - 1;
					
					if($lookup > 0 && isset($exprData[0]) && $exprData[0] !== false && $exprData[0] !== '')
						$effect = $this->getSpellEffect($lookup, $exprData[0]);
					else
						$effect = $this->getSpellEffect($spellRow['spellID'], $exprData[0]);
					
					$radius = $this->selectRow("SELECT `radiusBase` FROM `".$this->getTablePrefix()."_spellradius` WHERE `radiusID` =? LIMIT 1;", array($effect['EffectRadiusIndex']));
					$base = $radius['radiusBase'];
					
					if(in_array($op, $signs) && is_numeric($oparg) && is_numeric($base))
					{
						$equation = $base.$op.$oparg;
						eval("\$base = $equation;");
					}
					$str .= abs($base);
					break;
				case 'h':
					if($lookup > 0)
					{
						$spell = $this->selectRow("SELECT `SpellAuraOptionsId` FROM `".$this->getTablePrefix()."_spell` WHERE `spellID` = ? LIMIT 1;", array($lookup));
						$auraOptions = $this->getSpellAuraOptions($spell['SpellAuraOptionsId']);
					}
					else
						$auraOptions = $this->getSpellAuraOptions($spellRow['SpellAuraOptionsId']);
					
					$base = $auraOptions['procChance'];
	
					if(in_array($op, $signs) && is_numeric($oparg) && is_numeric($base))
					{
						$equation = $base.$op.$oparg;
						eval("\$base = $equation;");
					}
					$str .= abs($base);
					break;
				case 'f':
					$exprData[0] = $exprData[0] - 1;
					
					if($lookup > 0 && isset($exprData[0]) && $exprData[0] !== false && $exprData[0] !== '')
						$effect = $this->getSpellEffect($lookup, $exprData[0]);
					else
						$effect = $this->getSpellEffect($spellRow['spellID'], $exprData[0]);
	
					$base = $effect['EffectDamageMultiplier'];
	
					if(in_array($op, $signs) && is_numeric($oparg) && is_numeric($base))
					{
						$equation = $base.$op.$oparg;
						eval("\$base = $equation;");
					}
					$str .= abs($base);
					break;
				case 'n':
					if($lookup > 0)
					{
						$spell = $this->selectRow("SELECT `SpellAuraOptionsId` FROM `".$this->getTablePrefix()."_spell` WHERE `spellID` = ? LIMIT 1;", array($lookup));
						$auraOptions = $this->getSpellAuraOptions($spell['SpellAuraOptionsId']);
					}
					else
						$auraOptions = $this->getSpellAuraOptions($spellRow['SpellAuraOptionsId']);
	
					$base = $auraOptions['procCharges'];
	
					if(in_array($op, $signs) && is_numeric($oparg) && is_numeric($base))
					{
						$equation = $base.$op.$oparg;
						eval("\$base = $equation;");
					}
					$str .= abs($base);
					break;
				case 'd':
					if($lookup > 0)
					{
						$spell = $this->selectRow("SELECT `durationBase` FROM `".$this->getTablePrefix()."_spell` a, `".$this->getTablePrefix()."_spellduration` b WHERE a.DurationIndex = b.durationID AND a.spellID=? LIMIT 1;", array($lookup));
						@$base = ($spell['durationBase'] > 0 ? $spell['durationBase'] + 1 : 0);
					}
					else
						$base = ($lastduration['durationBase'] > 0 ? $lastduration['durationBase'] + 1 : 0);
	
					if($op && is_numeric($oparg) && is_numeric($base))
					{
						$equation = $base.$op.$oparg;
						eval("\$base = $equation;");
					}
					$str .= $this->spell_duration($base);
					break;
				case 'i':
					if($lookup > 0)
					{
						$spell = $this->selectRow("SELECT `SpellTargetRestrictionsId` FROM `".$this->getTablePrefix()."_spell` WHERE `spellID` = ? LIMIT 1;", array($lookup));
						if ($spell)
						{
							$targets = $this->getSpellTargetRestrictions($spell['SpellTargetRestrictionsId']);
							$base = (int)$targets['MaxAffectedTargets'];
						}
						else
							$base = 0;
					}
					else
					{
						$targets = $this->getSpellTargetRestrictions($spellRow['SpellTargetRestrictionsId']);
						$base = (int)$targets['MaxAffectedTargets'];
					}
					
					if($op && is_numeric($oparg) && is_numeric($base))
					{
						$equation = $base.$op.$oparg;
						eval("\$base = $equation;");
					}
					$str .= $base;
					break;
				case 'e':
					$exprData[0] = $exprData[0] - 1;
					
					if($lookup > 0 && isset($exprData[0]) && $exprData[0] !== false && $exprData[0] !== '')
						$effect = $this->getSpellEffect($lookup, $exprData[0]);
					else
						$effect = $this->getSpellEffect($spellRow['spellID'], $exprData[0]);
					
					$base = (float)$effect['EffectValueMultiplier'];
	
					if(in_array($op, $signs) && is_numeric($oparg) && is_numeric($base))
					{
						$equation = $base.$op.$oparg;
						eval("\$base = $equation;");
					}
	
					$str .= $base;
					$lastvalue = $base;
					break;
				case 'v':
					if($lookup > 0)
					{
						$spell = $this->selectRow("SELECT `SpellTargetRestrictionsId` FROM `".$this->getTablePrefix()."_spell` WHERE `spellID` = ? LIMIT 1;", array($lookup));
						if ($spell)
						{
							$targets = $this->getSpellTargetRestrictions($spell['SpellTargetRestrictionsId']);
							$base = (int)$targets['MaxTargetLevel'];
						}
						else
							$base = 0;
					}
					else
					{
						$targets = $this->getSpellTargetRestrictions($spellRow['SpellTargetRestrictionsId']);
						$base = (int)$targets['MaxTargetLevel'];
					}
	
					if($op && $oparg > 0 && $base > 0)
					{
						$equation = $base.$op.$oparg;
						eval("\$base = $equation;");
					}
					$str .= $base;
					break; 
				case 'u':
					if($lookup > 0)
					{
						$spell = $this->selectRow("SELECT `SpellAuraOptionsId` FROM `".$this->getTablePrefix()."_spell` WHERE `spellID` = ? LIMIT 1;", array($lookup));
						$auraOptions = $this->getSpellAuraOptions($spell['SpellAuraOptionsId']);
					}
					else
						$auraOptions = $this->getSpellAuraOptions($spellRow['SpellAuraOptionsId']);
					
					if(isset($auraOptions['StackAmount']))
						$base = $auraOptions['StackAmount'];
					
					if(in_array($op, $signs) && is_numeric($oparg) && is_numeric($base))
					{
						$equation = $base.$op.$oparg;
						eval("\$base = $equation;");
					}
					$str .= abs($base);
					$lastvalue = $base;
					break;
				case 'b': // only used at one spell (14179) should be 20, column 110/111/112?)
					$exprData[0] = $exprData[0] - 1;
					
					if($lookup > 0 && isset($exprData[0]) && $exprData[0] !== false && $exprData[0] !== '')
						$effect = $this->getSpellEffect($lookup, $exprData[0]);
					else
						$effect = $this->getSpellEffect($spellRow['spellID'], $exprData[0]);
					
					$base = (float)$effect['EffectPointsPerComboPoint'];
					 
					if(in_array($op, $signs) && is_numeric($oparg) && is_numeric($base))
					{
						$equation = $base.$op.$oparg;
						eval("\$base = $equation;");
					}
					$str .= abs($base);
					$lastvalue = $base;
					break;
				case 'l':
					if($lastvalue > 1)
						$str .= $exprData[1];
					else
						$str .= $exprData[0];
					break;
				case 'g':
					$str .= $exprData[0];
					break;
				default:
					$str .= "[{$var} ($op::$oparg::$lookup::$exprData[0])]";
			}
		}
		$str .= substr($data, $pos);
		
		//handle those anomalies, might be there for a reason but i just dont see it
		while (preg_match('/\}(\.\d+)/', $str, $result, PREG_OFFSET_CAPTURE))
		{
			$str = substr($str, 0, $result[1][1]) . substr($str, $result[1][1] + strlen($result[1][0]));
		}
	
		$str = @preg_replace_callback("|\{([^\}]+)\}|", create_function('$matches', 'return eval("return abs(".$matches[1].");");'), $str);
		
		return $str;
	}
	
	/*
		These are statements used to include effects from glyphs and such into the tooltip
	*/
	private function handleDescriptionIfStatements($string)
	{
		$pos = 0;
		$str = '';
		while(false!==($npos=strpos($string, '$?', $pos)))
		{
			if($npos!=$pos)
				$str .= substr($string, $pos, $npos-$pos);
			$pos = $npos+1;
	
			if('$?' == substr($string, $pos, 1))
			{
				$str .= '$?';
				$pos++;
				continue;
			}
			
			//there are more possible scenarios but im gonna go with just this simple one since we only need it for talents
			if (!preg_match('/^\?[\(]*(\w)?(\d+)?[\)]*\[([\n]*.*?)\]\[([\n]*.*?)\]/', substr($string, $pos), $result))
				continue;
			
			$exprType = $result[1];
			$lookup = $result[2];
			$option1 = $result[3];
			$option2 = $result[4];
			
			if(!$option1 && !$option2)
				continue;
			
			$pos += strlen($result[0]);
			
			//we dont need to check any of those stuff for our tooltips
			if (!empty($option2) && $option2 != '')
				$str .= $option2;
			else
				$str .= $option1;
		}
		$str .= substr($string, $pos);

		return $str;
	}
	
	public function getSpellTooltip($spell)
	{
		$query = $this->db->query("
				SELECT `spellID`, `".$this->getTablePrefix()."_spellicons`.`iconname`, `SpellName` AS spellname, `Rank` AS rank, `ToolTip` AS tooltip, `CastingTimeIndex`, `DurationIndex`, `SpellAuraOptionsId`, `SpellCooldownsId`, `SpellEquippedItemsId`, `SpellInterruptsId`, `DurationIndex`, `powerType`, `rangeIndex`, `runeCostID`, `SpellPowerId`, `SpellTargetRestrictionsId`, `SpellClassOptionsId`
				FROM `".$this->getTablePrefix()."_spell` s, `".$this->getTablePrefix()."_spellicons`
				WHERE
					`spellID` = ?
					AND `".$this->getTablePrefix()."_spellicons`.`id` = `SpellIconID`
				LIMIT 1;
			",
			array($spell)
		);
		
		if($query->num_rows() > 0)
		{
			$result = $query->result_array();
			
			return $this->parseSpellTooltip($result[0]);
		}
		
		unset($query);
		
		return false;	
	}
	
	public function parseSpellTooltip($row)
	{
		//Cast time
		$castRow = false;
		if ((int)$row['CastingTimeIndex'] > 1)
		{
			$castRow = $this->selectRow("SELECT `CastTime` FROM `".$this->getTablePrefix()."_spellcasttimes` WHERE `id` = ? LIMIT 1;", $row['CastingTimeIndex']);
			$casttime = (int)$castRow['CastTime']/1000;
		}
		
		//Cooldown
		$cooldown = false;
		if ((int)$row['SpellCooldownsId'] > 0)
		{
			$cooldownRow = $this->selectRow("SELECT `CategoryRecoveryTime`, `RecoveryTime` FROM `".$this->getTablePrefix()."_spellcooldowns` WHERE `Id` = ? LIMIT 1;", $row['SpellCooldownsId']);
			
			//not sure about how this works
			if ((int)$cooldownRow['RecoveryTime'] > 0)
				$cooldown = (int)$cooldownRow['RecoveryTime'];
			else
				$cooldown = (int)$cooldownRow['CategoryRecoveryTime'];
		}
		
		//Equipment requirements
		$EquippedItemClass = false;
		if ((int)$row['SpellEquippedItemsId'] > 0)
		{
			$equipReqRow = $this->selectRow("SELECT `EquippedItemClass` FROM `".$this->getTablePrefix()."_spellequippeditems` WHERE `Id` = ? LIMIT 1;", $row['SpellEquippedItemsId']);
			$EquippedItemClass = (int)$equipReqRow['EquippedItemClass'];
		}
		
		//Spell Interrupts
		//We need this to check if it's channeled
		$ChannelInterruptFlags = false;
		if ((int)$row['SpellInterruptsId'] > 0)
		{
			$interruptRow = $this->selectRow("SELECT `ChannelInterruptFlags` FROM `".$this->getTablePrefix()."_spellinterrupts` WHERE `Id` = ? LIMIT 1;", $row['SpellInterruptsId']);
			$ChannelInterruptFlags = (int)$interruptRow['ChannelInterruptFlags'];
		}
		
		//Range
		$range = false;
		if ((int)$row['rangeIndex'] > 0)
		{
			$rangeRow = $this->selectRow("SELECT `rangeMax` FROM `".$this->getTablePrefix()."_spellrange` WHERE `rangeID` = ? LIMIT 1;", $row['rangeIndex']);
			$range = $rangeRow['rangeMax'];
		}
		
		//Power consumation
		$powerRow = false;
		if ((int)$row['SpellPowerId'] > 0)
			$powerRow = $this->selectRow("SELECT `manaCost`, `ManaCostPercentage` FROM `".$this->getTablePrefix()."_spellpower` WHERE `id` = ? LIMIT 1;", $row['SpellPowerId']);
		
		//Rune cost
		$runeRow = false;
		if ((int)$row['runeCostID'] > 0)
			$runeRow = $this->selectRow("SELECT `RuneCost1` AS blood, `RuneCost2` AS unholy, `RuneCost3` AS frost FROM `".$this->getTablePrefix()."_spellrunecost` WHERE `id` = ? LIMIT 1;", $row['runeCostID']);
		
		//decide if the cast should be displayed
		$SpellCastableSigns = 0;
		
		$ForcedCastables = array(59672,50720);
		
		if ($cooldown && $cooldown > 0)
			$SpellCastableSigns = $SpellCastableSigns + 2;
		if ($powerRow && ($powerRow['manaCost'] > 0 || $powerRow['ManaCostPercentage'] > 0))
			$SpellCastableSigns = $SpellCastableSigns + 2;
		
		//Spell that costs 1 runic power seems wrong...
		if ($row['powerType'] == 6 && $powerRow['manaCost'] == 10)
			$SpellCastableSigns = $SpellCastableSigns - 1;
		
		if ($castRow || $ChannelInterruptFlags > 0)
			$SpellCastableSigns++;
		if ($runeRow && ($runeRow['blood'] > 0 || $runeRow['frost'] > 0 || $runeRow['unholy'] > 0))
			$SpellCastableSigns++;
		if ($range)
			$SpellCastableSigns++;
			
		$isCastableSpell = (in_array((int)$row['spellID'], $ForcedCastables) || $SpellCastableSigns >= 2);
		
		$hasPower = (($powerRow && ($powerRow['manaCost'] > 0 || $powerRow['ManaCostPercentage'] > 0)) || ($runeRow && ($runeRow['blood'] > 0 || $runeRow['frost'] > 0 || $runeRow['unholy'] > 0)));
		
		//Reagents
		//Requires world database
		/*
		$reagents = array();
		$i=0;
		for($j = 1; $j <= 8; $j++)
		{
			if($row['reagent'.$j] > 0)
			{
				$reagents[$i] = array();
				//Get the reagent name
				$names = $this->selectRow('
					SELECT name{, l.name_loc?d as `name_loc`}
					FROM item_template i
					{ LEFT JOIN (locales_item l) ON l.entry = i.entry AND ? }
					WHERE i.entry = ?d',
					($_SESSION['locale']>0)? $_SESSION['locale']: DBSIMPLE_SKIP,
					($_SESSION['locale']>0)? 1: DBSIMPLE_SKIP,
					$row['reagent'.$j]
				);
				$reagents[$i]['name'] = localizedName($names);
				//Increase the reagents count
				$reagents[$i]['count'] = abs($row['reagentcount'.$j]);
				$i++;
			}
		}
		*/
		
		//Required Tools
		//Requires world database
		/*
		$tools = array();
		$i=0;
		for($j=1;$j<=2;$j++)
		{
			if($row['tool'.$j])
			{
				$tools[$i] = array();
				//Get the tool name
				$names = $DB->selectRow('
					SELECT name{, l.name_loc?d as `name_loc`}
					FROM item_template i
					{ LEFT JOIN (locales_item l) ON l.entry = i.entry AND ? }
					WHERE i.entry = ?d',
					($_SESSION['locale']>0)? $_SESSION['locale']: DBSIMPLE_SKIP,
					($_SESSION['locale']>0)? 1: DBSIMPLE_SKIP,
					$row['tool'.$j]
				);
				$tools[$i]['name'] = localizedName($names);
				$i++;
			}
		}
		*/
		
		// Parse the spell description
		$desc = $this->parseSpellDesc($row);
	
		$x = '';
		$x .= '<div style="max-width: 320px;"><table class="spell-tooltip-main" style="width: 100%;" cellpadding="0" cellspacing="0"><tr><td>';
	
		if ($row['rank'])
			$x .= '<table width="100%" cellpadding="0" cellspacing="0"><tr><td>';
	
		$x .= '<b id="spell-tooltip-name" style="font-size: 15px;">'.$row['spellname'].'</b><br />';
	
		if ($row['rank'])
			$x .= '</td><th align="right"><b id="spell-tooltip-rank" class="q0">'.$row['rank'].'</b></th></tr></table>';
	
		if ($range && $hasPower)
			$x .= '<table width="100%" cellpadding="0" cellspacing="0"><tr><td>';
	
		if ($isCastableSpell && $runeRow && ($runeRow['blood'] > 0 || $runeRow['frost'] > 0 || $runeRow['unholy'] > 0))
		{
			if ($runeRow['blood'] > 0)
				$x .= $runeRow['blood'].' Blood ';
			if ($runeRow['frost'] > 0)
				$x .= $runeRow['frost'].' Frost ';
			if ($runeRow['unholy'] > 0)
				$x .= $runeRow['unholy'].' Unholy';
		}
		else if ($isCastableSpell)
		{
			if ($powerRow && $powerRow['manaCost'] > 0)
			{
				switch ($row['powerType'])
				{
					case 0: //mana
						$x .= $powerRow['manaCost'].' Mana<br />';
						break;
					case 1: //rage
						$x .= ($powerRow['manaCost'] / 10).' Rage<br />';
						break;
					case 2: //focus
						$x .= ($powerRow['manaCost']).' Focus<br />';
						break;
					case 3: //energy
						$x .= ($powerRow['manaCost']).' Energy<br />';
						break;
					case 6: //runic power
						$x .= ($powerRow['manaCost'] / 10).' Runic Power<br />';
						break;
				}
			}
	
			if ($powerRow && $powerRow['ManaCostPercentage'] > 0)
				$x .= $powerRow['ManaCostPercentage']."% of base mana</br>";
		}
	
		if ($range && $hasPower)
			$x .= '</td><th align="right">';
	
		if ($isCastableSpell && $range)
		{
			if ($range <= 5)
				$x .= 'Melee Range<br />';
			else
				$x .= $range.' yd range<br />';
		}
		
		if ($range && $hasPower)
			$x .= '</th></tr></table>';
		
		if ($isCastableSpell)
		{
			if (($ChannelInterruptFlags || $castRow || $row['CastingTimeIndex'] == 1) && $cooldown)
				$x .= '<table width="100%" cellpadding="0" cellspacing="0"><tr><td>';
		
			if ($ChannelInterruptFlags > 0)
				$x .= 'Channeled';
			elseif (isset($casttime))
			{
				if ($casttime > 0)
					$x .= $casttime.' sec cast';
				else
					$x .= 'Instant cast';
			}
			elseif ($row['CastingTimeIndex'] == 1)
				$x .= 'Instant cast';
		
			if (($ChannelInterruptFlags || $castRow || $row['CastingTimeIndex'] == 1) && $cooldown)
				$x .= '</td><th align="right">';
			
			if ($cooldown && $cooldown > 0)
			{
				if (($cooldown/1000 < 60))
					$x.= ($cooldown/1000).' sec cooldown';
				elseif ($cooldown/1000 >= 60)
					$x.= ($cooldown/1000/60).' min cooldown';
			}
			
			if(($ChannelInterruptFlags || $castRow || $row['CastingTimeIndex'] == 1) && $cooldown)
				$x .= '</th></tr></table>';
		}
	
		$x .= '</td></tr></table>';
		
		/*
		if($reagents)
		{
			$x .= '<table><tr><td>';
			$x .= 'Reagents: ';
			foreach($reagents as $i => $reagent)
			{
				$x .= $reagent['name'];
				if($reagent['count']>1)
					$x .= ' ('.$reagents[$i]['count'].')';
				if(!($i>=(count($reagents)-1)))
					$x .= ', ';
				else
					$x .= '<br>';
			}
			$x .= '</td></tr></table>';
		}
	
		if($tools)
		{
			$x .= '<table><tr><td>';
			$x .= 'Tools: ';
			foreach($tools as $i => $tool)
			{
				$x .= $tool['name'];
				if(!($i>=(count($tools)-1)))
					$x .= ', ';
				else
					$x .= '<br>';
			}
			$x .= '</td></tr></table>';
		}
		*/
		
		//decide if we need to display spell requirements
		if ($isCastableSpell)
		{
			if ($EquippedItemClass && $EquippedItemClass == 2)
				$x .= '<table class="spell-tooltip-requires" cellpadding="0" cellspacing="0"><tr><td>Requires Melee Weapon</td></tr></table>';
		}
			
		if($desc && $desc <> '_empty_')
			$x .= '<table class="spell-tooltip-description" cellpadding="0" cellspacing="0"><tr><td><span class="q">'.$desc.'</span></td></tr></table>';
		
		$x .= '</div>';
		
		return $x;
	}
	
	private function spell_duration($base)
	{
		$seconds = round($base/1000);
		
		//if it's a minure or greater
		if ($seconds % 60 == 0)
			return ($seconds / 60).' min';
		
		return $seconds.' sec';
	}
}