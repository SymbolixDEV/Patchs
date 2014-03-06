<?php

class Wotlk_model extends CI_Model
{
	public $realm;
	private $debug = false;
	
	public function __construct()
	{
		parent::__construct();
	}
	
	public function setRealm($id)
	{
		$this->realm = $this->realms->getRealm($id);
	}
	
	public function enableDebugMode()
	{
		$this->debug = true;
	}
	
	public function Exists($spell)
	{
		$this->db->select('COUNT(*)')->from('data_wotlk_spell')->where(array('spellID' => $spell));
		$query = $this->db->get();
		
		if($query->num_rows() > 0)
			return true;
		
		unset($query);
		
		return false;
	}
	
	public function glyphExists($id)
	{
		$this->db->select('COUNT(*)')->from('data_wotlk_glyphproperties')->where(array('id' => $id));
		$query = $this->db->get();
		
		if($query->num_rows() > 0)
			return true;
		
		unset($query);
		
		return false;
	}
	
	public function getSpellInfo($spell)
	{
		$query = $this->db->query("SELECT `spellname` AS name, `rank`, `tooltip` FROM `data_wotlk_spell` WHERE `spellID` = ? LIMIT 1;", array($spell));
		
		if($query->num_rows() > 0)
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
	
	private function getSpellDuration($id)
	{
		$query = $this->db->query('SELECT * FROM `data_wotlk_spellduration` WHERE `durationID` = ? LIMIT 1', array($id));
		
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
		$query = $this->db->query('SELECT * FROM `data_wotlk_spellrange` WHERE `rangeID` = ? LIMIT 1', array($id));
		
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
		$query = $this->db->query('
				SELECT `spellID`, `iconname`, `tooltip`, `spellname`, `rank`, `rangeID`, `manacost`, `manacostpercent`, `spellcasttimesID`, `stack`, `effect1BasePoints`, `effect2BasePoints`, `effect3BasePoints`, `effect1Amplitude`, `effect2Amplitude`, `effect3Amplitude`, `effect1DieSides`, `effect2DieSides`, `effect3DieSides`, `effect1ChainTarget`, `effect2ChainTarget`, `effect3ChainTarget`, `effect1radius`, `effect2radius`, `effect3radius`, `effect1MiscValue`, `effect2MiscValue`, `effect3MiscValue`, `ChannelInterruptFlags`, `procChance`, `procCharges`, `effect_1_proc_chance`, `effect_2_proc_chance`, `effect_3_proc_chance`, `effect1itemtype`, `effect1Aura`, `spellTargets`, `dmg_multiplier1`, `durationID`
				FROM `data_wotlk_spell` s, `data_wotlk_spellicons`
				WHERE
					`spellID` = ?
					AND `id` = spellicon
				LIMIT 1
			',
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
		if(isset($spellRow['durationBase']))
			$lastduration = array('durationBase' => $spellRow['durationBase']);
		else
			$lastduration = $this->getSpellDuration($spellRow['durationID']);
		
		$signs = array('+', '-', '/', '*', '%', '^');
	
		$data = $spellRow['tooltip'];
		
		if ($this->debug)
			var_dump($data);
		
		//handle them if statements
		$data = $this->handleDescriptionIfStatements($data);
		
		$data = strtr($data, array("\r" => '', "\n" => '<br />'));
		$data = preg_replace('/\|cff([a-f0-9]{6})(.+?)\|r/i', '<span style="color: #$1;">$2</span>', $data);
		
		$pos = 0;
		$str = '';
		while(false!==($npos=strpos($data, '$', $pos)))
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
				$exprData[0] = 1;
	
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
				var_dump($exprType);
			
			switch($exprType)
			{
				case 'r':
					if(!isset($spellRow['rangeMax']))
						$spellRow = array_merge($spellRow, $this->getSpellRange($spellRow['rangeID']));
	
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
					if($lookup > 0 && $exprData[0])
						$spell = $this->selectRow('SELECT `effect'.$exprData[0].'BasePoints` FROM `data_wotlk_spell` WHERE `spellID` = ? LIMIT 1', array($lookup));
					else
						$spell = $spellRow;
	
					$base = $spell['effect'.$exprData[0].'BasePoints']+1;
	
					if(in_array($op, $signs) && is_numeric($oparg) && is_numeric($base))
					{
						$equation = $base.$op.$oparg;
						eval("\$base = $equation;");
					}
	
					$str .= $base;
					$lastvalue = $base;
					break;
				case 's':
					if($lookup > 0 && $exprData[0])
						$spell = $this->selectRow('SELECT `effect'.$exprData[0].'BasePoints`, `effect'.$exprData[0].'DieSides` FROM `data_wotlk_spell` WHERE `spellID` = ? LIMIT 1', array($lookup));
					else
						$spell = $spellRow;
	
					if(!$exprData[0]) $exprData[0]=1;
						$base = (int)$spell['effect'.$exprData[0].'BasePoints']+1;
					
					if(in_array($op, $signs) && is_numeric($oparg) && is_numeric($base))
					{
						$equation = $base.$op.$oparg;
						eval("\$base = $equation;");
					}
					
					if (abs($base) > 0 && abs($base) < 0.9)
						$base = round($base, 1);
					
					@$str .= abs($base).((int)$spell['effect'.$exprData[0].'DieSides'] > 1 ? ' to '.abs(($base + $spell['effect'.$exprData[0].'DieSides'])) : '');
					$lastvalue = $base;
					break;
				case 'o':
					if($lookup > 0 && $exprData[0])
					{
						$spell = $this->selectRow('SELECT `durationID`, `effect'.$exprData[0].'BasePoints`, `effect'.$exprData[0].'Amplitude`, `effect'.$exprData[0].'DieSides` FROM `data_wotlk_spell` WHERE `spellID` = ? LIMIT 1', array($lookup));
						$lastduration = $this->selectRow('SELECT * FROM `data_wotlk_spellduration` WHERE `durationID` = ? LIMIT 1', array($spell['durationID']));
					}
					else
						$spell = $spellRow;
	
					if(!$exprData[0]) $exprData[0] = 1;
					$base = $spell['effect'.$exprData[0].'BasePoints']+1;
	
					if($spell['effect'.$exprData[0].'Amplitude'] <= 0) $spell['effect'.$exprData[0].'Amplitude'] = 5000;
	
					$str .= (($lastduration['durationBase'] / $spell['effect'.$exprData[0].'Amplitude']) * abs($base).($spell['effect'.$exprData[0].'DieSides'] > 1 ? '-'.abs(($base+$spell['effect'.$exprData[0].'DieSides'])) : ''));
					break;
				case 't':
					if($lookup > 0 && $exprData[0])
						$spell = $this->selectRow('SELECT * FROM `data_wotlk_spell` WHERE `spellID` = ? LIMIT 1', array($lookup));
					else
						$spell = $spellRow;
	
					if(!$exprData[0]) $exprData[0] = 1;
						$base = $spell['effect'.$exprData[0].'Amplitude']/1000;
	
					// TODO!!
					if($base==0)	$base=1;
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
					if($lookup > 0 && $exprData[0])
						$spell = $this->selectRow('SELECT `effect'.$exprData[0].'BasePoints` FROM `data_wotlk_spell` WHERE `spellID` = ? LIMIT 1', array($lookup));
					else
						$spell = $spellRow;
	
					// TODO!!
					if(!$exprData[0]) $exprData[0] = 1;
	
					$base = $spell['effect'.$exprData[0].'BasePoints']+1;
	
					if(in_array($op, $signs) && is_numeric($oparg) && is_numeric($base))
					{
						$equation = $base.$op.$oparg;
						eval("\$base = $equation;");
					}
					$str .= abs($base);
					$lastvalue = $base;
					break;
				case 'x':
					if($lookup > 0 && $exprData[0])
						$spell = $this->selectRow('SELECT `effect'.$exprData[0].'ChainTarget` FROM `data_wotlk_spell` WHERE `spellID` = ? LIMIT 1', array($lookup));
					else
						$spell = $spellRow;
	
					$base = $spell['effect'.$exprData[0].'ChainTarget'];
	
					if(in_array($op, $signs) && is_numeric($oparg) && is_numeric($base))
					{
						$equation = $base.$op.$oparg;
						eval("\$base = $equation;");
					}
					$str .= abs($base);
					$lastvalue = $base;
					break;
				case 'q':
					if($lookup > 0 && $exprData[0])
						$spell = $this->selectRow('SELECT `effect'.$exprData[0].'MiscValue` FROM `data_wotlk_spell` WHERE `spellID` = ? LIMIT 1', array($lookup));
					else
						$spell = $spellRow;
	
					if(!($exprData[0]))
						$exprData[0]=1;
					$base = $spell['effect'.$exprData[0].'MiscValue'];
	
					if(in_array($op, $signs) && is_numeric($oparg) && is_numeric($base))
					{
						$equation = $base.$op.$oparg;
						eval("\$base = $equation;");
					}
					$str .= abs($base);
					$lastvalue = $base;
					break;
				case 'a':
					if($lookup > 0 && $exprData[0])
						$spell = $this->selectRow('SELECT `effect1radius`, `effect2radius`, `effect3radius` FROM `data_wotlk_spell` WHERE `spellID` = ? LIMIT 1', array($lookup));
					else
						$spell = $spellRow;
	
					$exprData[0] = 1; // TODO
					//old $radius = $DB->selectCell('SELECT radiusBase FROM ?_spellradius WHERE radiusID=? LIMIT 1', $spell['effect'.$exprData[0].'radius']);
					$radius = $this->selectRow('SELECT `radiusBase` FROM `data_wotlk_spellradius` WHERE `radiusID` =? LIMIT 1', array($spell['effect'.$exprData[0].'radius']));
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
						$spell = $this->selectRow('SELECT `procChance` FROM `data_wotlk_spell` WHERE `spellID` = ? LIMIT 1', array($lookup));
					else
						$spell = $spellRow;
	
					$base = $spell['procChance'];
	
					if(in_array($op, $signs) && is_numeric($oparg) && is_numeric($base))
					{
						$equation = $base.$op.$oparg;
						eval("\$base = $equation;");
					}
					$str .= abs($base);
					break;
				case 'f':
					if($lookup > 0 && $exprData[0])
						$spell = $this->selectRow('SELECT `dmg_multiplier'.$exprData[0].'` FROM `data_wotlk_spell` WHERE `spellID` = ? LIMIT 1', array($lookup));
					else
						$spell = $spellRow;
	
					$base = $spell['dmg_multiplier'.$exprData[0]];
	
					if(in_array($op, $signs) && is_numeric($oparg) && is_numeric($base))
					{
						$equation = $base.$op.$oparg;
						eval("\$base = $equation;");
					}
					$str .= abs($base);
					break;
				case 'n':
					if($lookup > 0)
						$spell = $this->selectRow('SELECT `procCharges` FROM `data_wotlk_spell` WHERE `spellID` = ? LIMIT 1', array($lookup));
					else
						$spell = $spellRow;
	
					$base = $spell['procCharges'];
	
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
						$spell = $this->selectRow('SELECT `durationBase` FROM `data_wotlk_spell` a, `data_wotlk_spellduration` b WHERE a.durationID = b.durationID AND a.spellID=? LIMIT 1', array($lookup));
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
						$spell = $this->selectRow('SELECT `spellTargets` FROM `data_wotlk_spell` WHERE `spellID` = ? LIMIT 1', array($lookup));
						$base = ($spell ? $spell['spellTargets'] : 0);
					}
					else
						$base = $spellRow['spellTargets'];
					
					if($op && is_numeric($oparg) && is_numeric($base))
					{
						$equation = $base.$op.$oparg;
						eval("\$base = $equation;");
					}
					$str .= $base;
					break;
				case 'e':
					if($lookup > 0 && $exprData[0])
						$spell = $this->selectRow('SELECT `effect_'.$exprData[0].'_proc_value` FROM `data_wotlk_spell` WHERE `spellID` = ? LIMIT 1', array($lookup));
					else
						$spell = $spellRow;
					
					$base = $spell['effect_'.$exprData[0].'_proc_value'];
	
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
						$spell = $this->selectRow('SELECT `affected_target_level` FROM `data_wotlk_spell` WHERE `spellID` = ? LIMIT 1', array($lookup));
						$base = ($spell ? $spell['affected_target_level'] : 0);
					}
					else
						$base = $spellRow['affected_target_level'];
					
					if($op && $oparg > 0 && $base > 0)
					{
						$equation = $base.$op.$oparg;
						eval("\$base = $equation;");
					}
					$str .= $base;
					break; 
				case 'u':
					if($lookup > 0)
						$spell = $this->selectRow('SELECT `stack` FROM `data_wotlk_spell` WHERE `spellID` = ? LIMIT 1', array($lookup));
					else
						$spell = $spellRow;
					
					if(isset($spell['stack']))
						$base = $spell['stack'];
					
					if(in_array($op, $signs) && is_numeric($oparg) && is_numeric($base))
					{
						$equation = $base.$op.$oparg;
						eval("\$base = $equation;");
					}
					$str .= abs($base);
					$lastvalue = $base;
					break;
				case 'b': // only used at one spell (14179) should be 20, column 110/111/112?)
					if($lookup > 0 && $exprData[0])
						$spell = $this->selectRow('SELECT `effect_'.$exprData[0].'_proc_chance` FROM `data_wotlk_spell` WHERE `spellID` = ? LIMIT 1', array($lookup));
					else
						$spell = $spellRow;
					
					$base = $spell['effect_'.$exprData[0].'_proc_chance'];
					 
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
		$query = $this->db->query('
				SELECT `spellID`, `iconname`, `tooltip`, `spellname`, `rank`, `rangeID`, `powerType`, `runeCostID`, `manacost`, `manacostpercent`, `spellcasttimesID`, `RecoveryTime`, `CategoryRecoveryTime`, `stack`, `tool1`, `tool2`, `reagent1`, `reagent2`, `reagent3`, `reagent4`, `reagent5`, `reagent6`, `reagent7`, `reagent8`, `effect1BasePoints`, `effect2BasePoints`, `effect3BasePoints`, `effect1Amplitude`, `effect2Amplitude`, `effect3Amplitude`, `effect1DieSides`, `effect2DieSides`, `effect3DieSides`, `effect1ChainTarget`, `effect2ChainTarget`, `effect3ChainTarget`, `reagentcount1`, `reagentcount2`, `reagentcount3`, `reagentcount4`, `reagentcount5`, `reagentcount6`, `reagentcount7`, `reagentcount8`, `EquippedItemClass`, `effect1radius`, `effect2radius`, `effect3radius`, `effect1MiscValue`, `effect2MiscValue`, `effect3MiscValue`, `ChannelInterruptFlags`, `procChance`, `procCharges`, `effect_1_proc_chance`, `effect_2_proc_chance`, `effect_3_proc_chance`, `effect1itemtype`, `effect1Aura`, `spellTargets`, `dmg_multiplier1`, `durationID`
				FROM `data_wotlk_spell` s, `data_wotlk_spellicons`
				WHERE
					`spellID` = ?
					AND `id` = spellicon
				LIMIT 1
			',
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
		if((int)$row['spellcasttimesID'] > 1)
		{
			$castRow = $this->selectRow('SELECT `base` FROM `data_wotlk_spellcasttimes` WHERE `id` = ? LIMIT 1;', $row['spellcasttimesID']);
			$casttime = (int)$castRow['base']/1000;
		}
		
		//not sure how this works
		if ((int)$row['RecoveryTime'] > 0)
			$row['cooldown'] = (int)$row['RecoveryTime'];
		else
			$row['cooldown'] = (int)$row['CategoryRecoveryTime'];
		
		//Range
		$range = false;
		if ((int)$row['rangeID'] > 0)
		{
			$rangeRow = $this->selectRow('SELECT `rangeMax` FROM `data_wotlk_spellrange` WHERE `rangeID` = ? LIMIT 1;', $row['rangeID']);
			$range = $rangeRow['rangeMax'];
		}
		
		//Rune cost
		$runeRow = false;
		if ((int)$row['runeCostID'] > 0)
			$runeRow = $this->selectRow("SELECT `RuneCost1` AS blood, `RuneCost2` AS unholy, `RuneCost3` AS frost FROM `data_wotlk_spellrunecost` WHERE `id` = ? LIMIT 1;", $row['runeCostID']);
		
		//decide if the cast should be displayed
		$SpellCastableSigns = 0;
		
		$ForcedCastables = array(50720, 47585, 55050);
		
		if ($row['cooldown'] > 0)
			$SpellCastableSigns = $SpellCastableSigns + 2;
		if ($row['manacost'] > 0 || $row['manacostpercent'] > 0)
			$SpellCastableSigns = $SpellCastableSigns + 2;
		
		//Spell that costs 1 runic power seems wrong...
		if ($row['powerType'] == 6 && $row['manacost'] == 10)
			$SpellCastableSigns = $SpellCastableSigns - 1;
		
		//these are not always the signs of a castable
		if ($castRow || $row['ChannelInterruptFlags'] > 0)
			$SpellCastableSigns++;
		if ($runeRow && ($runeRow['blood'] > 0 || $runeRow['frost'] > 0 || $runeRow['unholy'] > 0))
			$SpellCastableSigns++;
		if ($range)
			$SpellCastableSigns++;
			
		$isCastableSpell = (in_array((int)$row['spellID'], $ForcedCastables) || $SpellCastableSigns >= 2);
		
		$hasPower = ($row['manacost'] > 0 || $row['manacostpercent'] > 0 || ($runeRow && ($runeRow['blood'] > 0 || $runeRow['frost'] > 0 || $runeRow['unholy'] > 0)));
		
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
	
		if($row['rank'])
			$x .= '<table width="100%" cellpadding="0" cellspacing="0"><tr><td>';
	
		$x .= '<b id="spell-tooltip-name" style="font-size: 15px;">'.$row['spellname'].'</b><br />';
	
		if($row['rank'])
			$x .= '</td><th align="right"><b id="spell-tooltip-rank" class="q0">'.$row['rank'].'</b></th></tr></table>';
	
		if($range && $hasPower)
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
			if ($row['manacost'] > 0)
			{
				switch ($row['powerType'])
				{
					case 0: //mana
						$x .= $row['manacost'].' Mana<br />';
						break;
					case 1: //rage
						$x .= ($row['manacost'] / 10).' Rage<br />';
						break;
					case 2: //focus
						$x .= ($row['manacost']).' Focus<br />';
						break;
					case 3: //energy
						$x .= ($row['manacost']).' Energy<br />';
						break;
					case 6: //runic power
						$x .= ($row['manacost'] / 10).' Runic Power<br />';
						break;
				}
			}
	
			if ($row['manacostpercent'] > 0)
				$x .= $row['manacostpercent']."% of base mana</br>";
		}
	
		if($range && $hasPower)
			$x .= '</td><th align="right">';
	
		if($isCastableSpell && $range)
		{
			if ($range <= 5)
				$x .= 'Melee Range<br />';
			else
				$x .= $range.' yd range<br />';
		}
		
		if($range && $hasPower)
			$x .= '</th></tr></table>';
		
		if ($isCastableSpell)
		{
			if (($row['ChannelInterruptFlags'] || isset($casttime) || $row['spellcasttimesID'] == 1) && $row['cooldown'])
				$x .= '<table width="100%" cellpadding="0" cellspacing="0"><tr><td>';
		
			if ($row['ChannelInterruptFlags'])
				$x .= 'Channeled';
			elseif (isset($casttime))
			{
				if ($casttime > 0)
					$x .= $casttime.' sec cast';
				else
					$x .= 'Instant cast';
			}
			elseif ($row['spellcasttimesID'] == 1)
				$x .= 'Instant cast';
		
			if (($row['ChannelInterruptFlags'] || isset($casttime) || $row['spellcasttimesID'] == 1) && $row['cooldown'])
				$x .= '</td><th align="right">';
	
			if (($row['cooldown']/1000 < 60) and ($row['cooldown'] != 0))
				$x.= ($row['cooldown']/1000).' sec cooldown';
			elseif ($row['cooldown']/1000 >= 60)
				$x.= ($row['cooldown']/1000/60).' min cooldown';
			
			if (($row['ChannelInterruptFlags'] || isset($casttime) || $row['spellcasttimesID'] == 1) && $row['cooldown'])
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
			if ((int)$row['EquippedItemClass'] == 2)
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