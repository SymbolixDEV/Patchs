<?php

class Armory_model extends CI_Model
{
	public $realm;
	private $connection;
	private $id;
	private $realmId;
	private $storedData;
	private $EmulatorSimpleString = '';
	
	public function __construct()
	{
		parent::__construct();
	}
	/**
	 * Assign the character ID to the model
	 */
	public function setId($id)
	{
		$this->id = $id;
	}

	/**
	 * Assign the realm object to the model
	 */
	public function setRealm($id)
	{
		$this->realmId = $id;
		$this->realm = $this->realms->getRealm($id);
		$this->EmulatorSimpleString =  str_replace(array('_ra', '_soap', '_rbac'), '', $this->realm->getConfig('emulator'));
	}
	
	private function getEmulatorString()
	{
		return $this->EmulatorSimpleString;
	}
	
	private function getEmulatorBuild()
	{
		switch ($this->getEmulatorString())
		{
			//cata 4.3.4
			case 'trinity_cata':
				return '15595';
			//cata 4.0.6a
			case 'skyfire':
			case 'arkcore':
				return '13633';
		}
		
		return false;
	}
	
	private function isCataclysm()
	{
		switch ($this->getEmulatorString())
		{
			case 'trinity_cata':
			case 'skyfire':
			case 'arkcore':
				return true;
		}
		
		return false;
	}
	
	/**
	 * Connect to the character database
	 */
	public function connect()
	{
		$this->realm->getCharacters()->connect();
		$this->connection = $this->realm->getCharacters()->getConnection();
	}

	/**
	 * Check if the current character exists
	 */
	public function characterExists()
	{
		$this->connect();

		$query = $this->connection->query("SELECT COUNT(*) AS total FROM ".table("characters", $this->realmId)." WHERE ".column("characters", "guid", false, $this->realmId)."= ?", array($this->id));
		$row = $query->result_array();

		if($row[0]['total'] > 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/**
	 * Get the character data that belongs to the character
	 */
	public function getCharacter()
	{
		$this->connect();

		$query = $this->connection->query($this->realm->getEmulator()->getQuery('get_character', $this->realmId), array($this->id));
		
		if($this->connection->_error_message())
		{
			die("Function getCharacter error: " . $this->connection->_error_message());
		}
		
		if($query && $query->num_rows() > 0)
		{
			$row = $query->result_array();

			return $row[0];
		}
		else
		{
			return array(
						"account" => "",
						"name" => "",
						"race" => "",
						"class" => "",
						"gender" => "",
						"level" => ""
					);
		}
	}

	/**
	 * Get the character stats that belongs to the character
	 */
	public function getStats()
	{
		$this->connect();
		
		$query = $this->connection->query("SELECT ".allColumns("character_stats", $this->realmId)." FROM ".table("character_stats", $this->realmId)." WHERE ".column("character_stats", "guid", false, $this->realmId)."= ?", array($this->id));
		
		if($this->connection->_error_message())
		{
			//die("Function getStats error: " . $this->connection->_error_message());
		}
		
		if($query && $query->num_rows() > 0)
		{
			$row = $query->result_array();

			return $row[0];
		}
		else
		{
			return false;
		}
	}

	/**
	 * Load items that belong to the character 
	 */
	public function getItems()
	{
		$this->connect();
		
		$query = $this->connection->query($this->realm->getEmulator()->getQuery("get_inventory_item"), array($this->id));

		if($query && $query->num_rows() > 0)
		{
			$row = $query->result_array();

			return $row;
		}
		else
		{
			return false;
		}
	}

	public function getGuild($guid = false)
	{
		if (!$guid)
			$guid = $this->id;
		
		$this->connect();

		$query = $this->connection->query("SELECT ".column("guild_member", "guildid", true, $this->realmId)." FROM ".table("guild_member", $this->realmId)." WHERE ".column("guild_member", "guid", false, $this->realmId)."= ?", array($guid));

		if($this->connection->_error_message())
		{
			die($this->connection->_error_message());
		}

		if($query && $query->num_rows() > 0)
		{
			$row = $query->result_array();

			return $row[0]['guildid'];
		}
		else
		{
			$query2 = $this->connection->query("SELECT ".column("guild", "guildid", true, $this->realmId)." FROM ".table("guild", $this->realmId)." WHERE ".column("guild", "leaderguid", false, $this->realmId)."= ?", array($guid));

			if($this->connection->_error_message())
			{
				//die($this->connection->_error_message());
			}

			if($query2 && $query2->num_rows() > 0)
			{

				$row2 = $query2->result_array();

				return $row2[0]['guildid'];
			}
			else
			{
				return false;
			}
		}
	}

	public function getGuildName($id)
	{
		if(!$id)
		{
			return '';
		}
		else
		{
			$this->connect();

			$query = $this->connection->query("SELECT ".column("guild", "name", true, $this->realmId)." FROM ".table("guild", $this->realmId)." WHERE ".column("guild", "guildid", false, $this->realmId)."= ?", array($id));

			if($query && $query->num_rows() > 0)
			{
				$row = $query->result_array();

				return $row[0]['name'];
			}
			else
			{
				return false;
			}
		}
	}
	
	public function getTalents($spec)
	{
		$this->connect();
		
		//Handle arcemu
		if ($this->getEmulatorString() == 'arcemu')
		{
			$this->getArcemuCharacterDataIfNeeded();
			
			if ($this->storedData)
			{
				//increase spec number
				$spec = $spec + 1;
				//get the talents string
				$talentList = $this->storedData['talents'.$spec];
				//check for empty talent list
				if ($talentList != '')
				{
					//strip the last ,
					$talentList = rtrim($talentList, ',');
					//convert to array
					$talents = explode(',', $talentList);
					//each second record in the array is the rank of the previous record, we need to merge them
					$new = array();
					foreach ($talents as $key => $value)
					{
						if (($key % 2) == 0) 	//push the talent id
							$new[] = array('talent_id' => $value);
						else 					//push the rank
							$new[count($new) - 1]['current_rank'] = $value;
					}
					unset($talents, $talentList);
					
					return $new;
				}
				else
					return false;
			}
			else
				return false;
		}
		
		$statements['trinity'] 		= 
		$statements['trinity_cata']	= 
		$statements['arkcore'] 		=
		$statements['skyfire'] 		= "SELECT `spell` FROM `character_talent` WHERE `guid` = ? AND `spec` = ? ORDER BY `spell` DESC;";
		
		$statements['mangos'] 		= 
		$statements['mangosr2'] 	= "SELECT `talent_id`, `current_rank` FROM `character_talent` WHERE `guid` = ? AND `spec` = ? ORDER BY `talent_id` DESC;";
		
		$query = $this->connection->query($statements[$this->getEmulatorString()], array($this->id, $spec));
		
		if($query && $query->num_rows() > 0)
		{
			$array = $query->result_array();
			
			//put the talents into a single array
			$temp = array();
			foreach ($array as $key => $row)
			{
				if (isset($row['spell']))
					$temp[] = $row['spell'];
				else
					$temp[] = $row;
			}
			unset($array);
			
			return $temp;
		}
		
		unset($query);
		
		return false;
	}
	
	public function getGlyphs($spec)
	{
		$this->connect();
		
		//Handle arcemu
		if ($this->getEmulatorString() == 'arcemu')
		{
			$this->getArcemuCharacterDataIfNeeded();
			
			if ($this->storedData)
			{
				//increase spec number
				$spec = $spec + 1;
				//get the talents string
				$glyphList = $this->storedData['glyphs'.$spec];
				//strip the last ,
				$glyphList = rtrim($glyphList, ',');
				//convert to array
				$glyphs = explode(',', $glyphList);
				//make it suitable to use the mangos function
				foreach ($glyphs as $key => $id)
				{
					$glyphs[$key] = array('glyph' => $id);
				}
				unset($glyphList, $key, $id);
				
				return $glyphs;
			}
			else
				return false;
		}
		
		$statements['trinity'] 		= 
		$statements['trinity_cata']	= 
		$statements['arkcore'] 		=
		$statements['skyfire'] 		= "SELECT * FROM `character_glyphs` WHERE `guid` = ? AND `spec` = ? LIMIT 1;";
		
		$statements['mangos'] 		= 
		$statements['mangosr2'] 	= "SELECT `slot`, `glyph` FROM `character_glyphs` WHERE `guid` = ? AND `spec` = ?;";
		
		$query = $this->connection->query($statements[$this->getEmulatorString()], array($this->id, $spec));

		if($query && $query->num_rows() > 0)
		{
			$array = $query->result_array();
			
			return $array;
		}
		
		unset($query);
		
		return false;
	}
	
	public function getTalentSpecsInfo()
	{
		$this->connect();
		
		//Handle arcemu
		if ($this->getEmulatorString() == 'arcemu')
		{
			$this->getArcemuCharacterDataIfNeeded();
			
			if ($this->storedData)
			{
				return array('speccount' => $this->storedData['speccount'], 'activespec' => $this->storedData['activespec']);
			}
			else
				return false;
		}
		
		$statements['trinity'] 		= 
		$statements['trinity_cata']	= 
		$statements['arkcore'] 		=
		$statements['skyfire'] 		= "SELECT `speccount`, `activespec` FROM `characters` WHERE `guid` = ? LIMIT 1;";
		
		$statements['mangos'] 		= 
		$statements['mangosr2'] 	= "SELECT `specCount` AS speccount, `activeSpec` AS activespec FROM `characters` WHERE `guid` = ? LIMIT 1;";
		
		$query = $this->connection->query($statements[$this->getEmulatorString()], array($this->id));

		if($query && $query->num_rows() > 0)
		{
			$result = $query->result_array();
			
			return $result[0];
		}
		
		unset($query);
		
		return false;
	}
	
	private function getArcemuCharacterDataIfNeeded()
	{
		if (!isset($this->storedData) || !is_array($this->storedData))
		{
			$query = $this->connection->query("SELECT `glyphs1`, `glyphs2`, `talents1`, `talents2`, `numspecs` AS speccount, `currentspec` AS activespec FROM `characters` WHERE `guid` = ? LIMIT 1;", array($this->id));

			if($query && $query->num_rows() > 0)
			{
				$array = $query->result_array();
				
				$this->storedData = $array[0];
				
				unset($array);
			}
			else
			{
				$this->storedData = false;
			}
			unset($query);
		}
	}
	
	/***************************************
	* 	 	  TOP ARENA FUNCTIONS
	***************************************/

	public function getTeam($type = 2)
	{
		//Switch the type number for arcemu
		if ($this->getEmulatorString() == 'arcemu')
		{
			switch ($type)
			{
				case 2: $type = 0; break;
				case 3: $type = 1; break;
				case 5: $type = 2; break;
			}
		}
		
		$this->connect();
		
		//First get the arena team member record for this character
		$statements['trinity'] = 
		$statements['trinity_cata']	= 
		$statements['arkcore'] =
		$statements['skyfire'] = "	SELECT 
										`arena_team_member`.`arenaTeamId` AS arenateamid, 
										`arena_team`.`name` AS teamName, 
										`arena_team`.`rating` AS teamRating, 
										`arena_team`.`rank` AS teamRank
									FROM `arena_team_member`, `arena_team` 
									WHERE `arena_team_member`.`guid` = ? AND `arena_team`.`arenaTeamId` = `arena_team_member`.`arenaTeamId` AND `arena_team`.`type` = ? 
									LIMIT 1;";
									
		//Mangos and MangosR2
		$statements['mangos'] = 
		$statements['mangosr2'] = "	SELECT 
										`arena_team_member`.`arenateamid`, 
										`arena_team`.`name` AS teamName, 
										`arena_team_stats`.`rating` AS teamRating, 
										`arena_team_stats`.`rank` AS teamRank
									FROM `arena_team_member`, `arena_team`, `arena_team_stats` 
									WHERE `arena_team_member`.`guid` = ? AND `arena_team`.`arenateamid` = `arena_team_member`.`arenateamid` AND `arena_team`.`arenateamid` = `arena_team_stats`.`arenateamid` AND `arena_team`.`type` = ? 
									LIMIT 1;";
		
		$statements['arcemu'] = "	SELECT 
										`id` AS arenateamid, 
										`rating` AS teamRating, 
										`ranking` AS teamRank, 
										`name` AS teamName 
									FROM `arenateams` 
									WHERE 
										(`type` = @type AND LOCATE(@guid, player_data1) = 1) OR
										(`type` = @type AND LOCATE(@guid, player_data2) = 1) OR
										(`type` = @type AND LOCATE(@guid, player_data3) = 1) OR
										(`type` = @type AND LOCATE(@guid, player_data4) = 1) OR
										(`type` = @type AND LOCATE(@guid, player_data5) = 1) OR
										(`type` = @type AND LOCATE(@guid, player_data6) = 1) OR
										(`type` = @type AND LOCATE(@guid, player_data7) = 1) OR
										(`type` = @type AND LOCATE(@guid, player_data8) = 1) OR
										(`type` = @type AND LOCATE(@guid, player_data9) = 1) OR
										(`type` = @type AND LOCATE(@guid, player_data10) = 1)
									LIMIT 1;";
		
		//set sql variables for arcemu
		if ($this->getEmulatorString() == 'arcemu')
		{
			$this->connection->query("SET @guid=?;", array($this->id));
			$this->connection->query("SET @type=?;", array($type));
		}
		
		$result = $this->connection->query($statements[$this->getEmulatorString()], array($this->id, $type));
		
		if($result && $result->num_rows() > 0)
		{
			$array = $result->result_array();
			//re-variable
			$team = $array[0];
			
			unset($array);
			
			// Get the team members
			$team['members'] = $this->getTeamMembers((int)$team['arenateamid']);
			//Find the player and remove him from the members array
			foreach ($team['members'] as $key => $member)
			{
				if ($member['guid'] == $this->id)
				{
					//store with new assoc key
					$team['player'] = $member;
					//remove from members
					unset($team['members'][$key]);
				}
			}
			
			return $team;
		}
		
		unset($result);
		
		return false;
	}

	private function getTeamMembers($teamId)
	{
		// Different handling for arcemu
		if ($this->getEmulatorString() == 'arcemu')
		{
			return $this->getTeamMembersArcemu($teamId);
		}
		
		$this->connect();
		
		//Trinity and Skyfire is the same
		$statements['trinity'] = 
		$statements['trinity_cata']	= 
		$statements['arkcore'] =
		$statements['skyfire'] = "	SELECT 
										`arena_team_member`.`guid`, 
										`arena_team_member`.`personalRating` AS rating,
										`arena_team_member`.`seasonGames` AS games,
										`arena_team_member`.`seasonWins` AS wins,
										`characters`.`name`,
										`characters`.`class`,
										`characters`.`race`,
										`characters`.`level`
									FROM `arena_team_member` 
									RIGHT JOIN `characters` ON `characters`.`guid` = `arena_team_member`.`guid` 
									WHERE `arena_team_member`.`arenateamid` = ? ORDER BY guid ASC;";
		
		//Mangos and MangosR2
		$statements['mangos'] = 
		$statements['mangosr2']	= "	SELECT 
										`arena_team_member`.`guid`, 
										`arena_team_member`.`personal_rating` AS rating,
										`arena_team_member`.`played_season` AS games,
										`arena_team_member`.`wons_season` AS wins,
										`characters`.`name`,
										`characters`.`class`,
										`characters`.`race`,
										`characters`.`level`
									FROM `arena_team_member` 
									RIGHT JOIN `characters` ON `characters`.`guid` = `arena_team_member`.`guid` 
									WHERE `arena_team_member`.`arenateamid` = ? ORDER BY guid ASC;";
		
		$result = $this->connection->query($statements[$this->getEmulatorString()], array($teamId));
		
		if($result && $result->num_rows() > 0)
		{
			$array = $result->result_array();
			
			//get some more info
			foreach ($array as $key => $row)
			{
				$array[$key]['className'] = $this->realms->getClass($row['class']);
				
				if (in_array($row['race'], array(4, 10)))
				{
					if ($row['race'] == 4)
						$row['raceName'] = "Night elf";
					else
						$row['raceName'] = "Blood elf";
				}
				else
					$row['raceName'] = $this->realms->getRace($row['race']);
					
				$array[$key]['raceName'] = $row['raceName'];
				
				//Try getting the player guild
				if ($guildid = $this->getGuild($row['guid']))
				{
					$row['guildName'] = $this->getGuildName($guildid);
				}
				unset($guildid);
				
				$array[$key]['guildName'] = (isset($row['guildName']) ? $row['guildName'] : false);
				$array[$key]['faction']	= $this->getFactionName($row['race']);
			}
			
			return $array;
		}
		
		unset($result);
		
		return false;
	}
	
	public function getTeamMembersArcemu($team)
	{
		$this->connect();
		
		$result = $this->connection->query("SELECT `player_data1`, `player_data2`, `player_data3`, `player_data4`, `player_data5`, `player_data6`, `player_data7`, `player_data8`, `player_data9`, `player_data10` FROM `arenateams` WHERE `id` = ? LIMIT 1;", array($team));
		
		if($result && $result->num_rows() > 0)
		{
			$members = array();
			$row = $result->result_array();
			$row = $row[0];
			
			// Get the team members
			for ($i = 1; $i <= 10; $i++)
			{
				if ($row['player_data'.$i] == '')
					continue;

				list($guid, $weekGames, $weekWins, $seasonGames, $seasonWins, $rating) = explode(' ', $row['player_data'.$i]);
				
				settype($guid, "integer");
				
				// Check if there is a player at this pos
				if ($guid == 0)
					continue;
				
				//Get some character data
				$result2 = $this->connection->query("SELECT `guid`, `name`, `class`, `race`, `level` FROM `characters` WHERE `guid` = ? LIMIT 1;", array($guid));
				
				if($result2 && $result2->num_rows() > 0)
				{
					$char = $result2->result_array();
					$char = $char[0];
					
					if (in_array($char['race'], array(4, 10)))
					{
						if ($char['race'] == 4)
							$char['raceName'] = "Night elf";
						else
							$char['raceName'] = "Blood elf";
					}
					else
						$char['raceName'] = $this->realms->getRace($char['race']);
					
					//Try getting the player guild
					if ($guildid = $this->getGuild($guid))
					{
						$char['guildName'] = $this->getGuildName($guildid);
					}
					unset($guildid);
					
					array_push($members, array(
						'guid' 		=> $guid,
						'rating'	=> $rating,
						'games'		=> $seasonGames,
						'wins'		=> $seasonWins,
						'name'		=> $char['name'],
						'class'		=> $char['class'],
						'className' => $this->realms->getClass($char['class']),
						'race'		=> $char['race'],
						'raceName'	=> $char['raceName'],
						'level'		=> $char['level'],
						'guildName' => (isset($char['guildName']) ? $char['guildName'] : false),
						'faction'	=> $this->getFactionName($char['race'])
					));
					
					unset($char);
				}
				unset($result2, $guid, $weekGames, $weekWins, $seasonGames, $seasonWins, $rating);
			}
			unset($row);
			
			//check if the team has any players
			if (count($members) > 0)
			{
				return $members;
			}
		}
		
		unset($result);
		
		return false;
	}
	
	private function getFactionName($id)
	{
		switch($id)
		{
			case 1: return 'alliance';
			case 2: return 'horde';
			case 3: return 'alliance';
			case 4: return 'alliance';
			case 5: return 'horde';
			case 6: return 'horde';
			case 7: return 'alliance';
			case 8: return 'horde';
			case 9: return 'horde';
			case 10: return 'horde';
			case 11: return 'alliance';
			case 22: return 'alliance';
		}
		
		return false;
	}
}