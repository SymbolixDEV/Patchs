<?php

class Character extends MX_Controller
{
	private $canCache;
	private $runtimeCache;
	
	private $js;
	private $css;
	private $id;
	private $realm;
	private $realmName;

	private $name;
	private $class;
	private $className;
	private $race;
	private $raceName;
	private $level;
	private $accountId;
	private $account;
	private $gender;

	private $stats;
	private $items;
	
	private $achievements;
	private $professions;
	
	private $talentTabs = array();
	private $talents = array();
	private $glyphs = array();
	private $talentSpecsInfo;
	
	private $TooltipPlayerData = array();
	
	private $enchantRegistry = array();
	private $gemRegistry = array();
	private $equippedItems = array();
	
	private $EmulatorSimpleString = '';
	
	function __construct()
	{
		parent::__construct();
		
		// Set JS and CSS paths
		$this->js = "modules/character/js/character.js";
		$this->css = "modules/character/css/character.css";

		$this->load->model("armory_model");
		
		$this->load->model("wotlk_dbc_model");
		$this->load->model("cata_dbc_model");
		
		$this->canCache = true;
		$this->items = array();
	}

	/**
	 * Initialize
	 */
	public function index($realm = false, $id = false)
	{
		if(!is_numeric($id))
		{
			$id = ucfirst($id);
			$id = $this->realms->getRealm($realm)->getCharacters()->getGuidByName($id);
		}

		$this->setId($realm, $id);
		
		if($this->id != false)
		{
			$this->getProfile();
		}
		else
		{
			$this->getError();
		}
	}
	
	public function getItem($id = false, $slot = 0)
	{
		if ($id != false)
		{
			$DisplayIdCache = $this->cache->get("items/item_displayid_".$this->realm."_".$id);
			
			if ($DisplayIdCache !== false)
			{
				$IconCache = $this->cache->get("items/display_".$DisplayIdCache);

				if ($IconCache != false)
				{
					return "<a 	href='" . $this->template->page_url . "item/" . $this->realm . "/" . $id . "' 
								".($this->config->item("use_fcms_tooltip") ? 'rel-e' : 'rel')."='item=".$id."' 
								data-item-slot='".$slot."' 
								data-realm='".$this->realm."'>
							</a><img src='https://wow.zamimg.com/images/wow/icons/large/".$IconCache.".jpg' />";
				}
				else
				{
					return "<a 	href='" . $this->template->page_url . "item/" . $this->realm . "/" . $id . "' 
								".($this->config->item("use_fcms_tooltip") ? 'rel-e' : 'rel')."='item=".$id."' 
								data-item-slot='".$slot."' 
								data-realm='".$this->realm."'>
							</a><img src='https://wow.zamimg.com/images/wow/icons/large/inv_misc_questionmark.jpg' />";
				}
			}
			else
			{
				$this->canCache = false;
				return $this->template->loadPage("icon_ajax.tpl", array('id' => $id, 'realm' => $this->realm, 'slot' => $slot, 'url' => $this->template->page_url));
			}
		}
	}
	
	/**
	 * Determinate which Id to assign
	 */
	public function setId($realm, $id)
	{

		// Check if valid X-Y format
		if(is_numeric($realm)
		&& is_numeric($id))
		{
			$this->id = $id;
			$this->realm = $realm;
			$this->EmulatorSimpleString = str_replace(array('_ra', '_soap', '_rbac'), '', $this->realms->getRealm($this->realm)->getConfig('emulator'));
			
			$this->armory_model->setRealm($this->realm);
			$this->armory_model->setId($this->id);
		}
		else
		{
			$this->realm = false;
			$this->id = false;
		}
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
	 * Get character info
	 */
	private function getInfo()
	{
		$character_data = $this->armory_model->getCharacter();
		
		if ($this->realms->getRealm($this->realm)->getEmulator()->hasStats())
		{
			$character_stats = $this->armory_model->getStats();
		}
		else
		{
			$character_stats = array('maxhealth' => lang("unknown", "character"));
		}
		
		//Get honor and conquest points for trinitycore cata
		if ($this->getEmulatorString() == 'trinity_cata' || $this->getEmulatorString() == 'skyfire' || $this->getEmulatorString() == 'arkcore')
		{
			$PVP = $this->armory_model->getPVPPoints();
			
			if ($PVP)
			{
				$character_data['totalHonorPoints'] = $PVP['totalHonorPoints'];
				$character_data['arenaPoints'] = $PVP['arenaPoints'];
			}
			unset($PVP);
		}
		
		$this->pvp = array(
						'kills' => (array_key_exists("totalKills", $character_data)) ? $character_data['totalKills'] : false,
						'honor' => (array_key_exists("totalHonorPoints", $character_data)) ? $character_data['totalHonorPoints'] : false,
						'arena' => (array_key_exists("arenaPoints", $character_data)) ? $character_data['arenaPoints'] : false
					);

		// Assign the character data as real variables
		foreach($character_data as $key=>$value)
		{
			$this->$key = $value;
		}

		// Assign the character stats
		$this->stats = $character_stats;
	
		// Get the account username
		$this->accountName = $this->internal_user_model->getNickname($this->account);

		$this->guild = $this->armory_model->getGuild();
		$this->guildName = $this->armory_model->getGuildName($this->guild);

		if(in_array($this->race, array(4,10)))
		{
			if($this->race == 4)
			{
				$this->raceName = "Night elf";
			}
			else
			{
				$this->raceName = "Blood elf";
			}
		}
		else
		{
			$this->raceName = $this->armory_model->realms->getRace($this->race);
		}

		$this->className = $this->armory_model->realms->getClass($this->class);
		$this->realmName = $this->armory_model->realm->getName();
		
		if($this->realms->getRealm($this->realm)->getEmulator()->hasStats())
		{
			// Find out which power field to use
			switch($this->className)
			{
				default:
					$this->secondBar = "mana";
					$this->secondBarValue = $this->stats['maxpower1'];
				break;

				case "Warrior":
					$this->secondBar = "rage";
					$this->secondBarValue = $this->stats['maxpower2'];
				break;

				case "Hunter":
					$this->secondBar = "focus";
					$this->secondBarValue = $this->stats['maxpower3'];
				break;

				case "Deathknight":
					$this->secondBar = "runic";
					$this->secondBarValue = $this->stats['maxpower7'];
				break;
			}
		}
		else
		{
			$this->secondBar = "mana";
			$this->secondBarValue = lang("unknown", "character");
		}

		// Load the items
		$items = $this->armory_model->getItems();

		// Item slots
		$slots = array(
					0 => "head",
					1 => "neck",
					2 => "shoulders",
					3 => "body",
					4 => "chest",
					5 => "waist",
					6 => "legs",
					7 => "feet",
					8 => "wrists",
					9 => "hands",
					10 => "finger1",
					11 => "finger2",
 					12 => "trinket1",
					13 => "trinket2",
					14 => "back",
					15 => "mainhand",
					16 => "offhand",
					17 => "ranged",
					18 => "tabard"
				);

		if (is_array($items))
		{
			// Loop through to assign the items
			foreach($items as $item)
			{
				//skyfire and trinity
				if (isset($item['enchantments']))
				{
					$this->TooltipPlayerData[$item['slot']] = array(
						'enchant' 			=> $this->GetItemEnchant($item['enchantments']),
						'gems'				=> $this->GetItemGems($item['enchantments']),
						'hasExtraSocket'	=> $this->hasExtraSocket($item['enchantments']),
					);
				}
				
				//add to the equipped items array so we can decide the item set pieces for them items
				$this->equippedItems[] = $item['itemEntry'];
				
				$this->items[$slots[$item['slot']]] = $this->getItem($item['itemEntry'], $item['slot']);
			}
		}

		// Loop through to make sure none are empty
		foreach($slots as $key => $value)
		{
			if(!array_key_exists($value, $this->items))
			{
				switch($value)
				{
					default: $image = $value; break;
					case "trinket1": $image = "trinket"; break;
					case "trinket2": $image = "trinket"; break;
					case "finger1": $image = "finger"; break;
					case "finger2": $image = "finger"; break;
					case "back": $image = "chest"; break;
				}

				$this->items[$value] = "<div class='item'><img src='".$this->template->page_url."application/images/armory/default/".$image.".gif' /></div>";
			}
		}
		
		//Get recent achievements
		$charRecentAchievements = $this->armory_model->getRecentAchievements();
		
		if ($charRecentAchievements)
		{
			$temp = array();
			
			//loop trough the char achievements and get some info
			foreach ($charRecentAchievements as $key => $achievementData)
			{
				//try getting some info about the achievement
				if ($achievementInfo = $this->getDbcModel()->getAchievementInfo($achievementData['achievement']))
				{
					//append the date of the achievement
					$achievementInfo['date'] = date("d/m/Y", $achievementData['date']);
					//append to the achievements
					$temp[] = $achievementInfo;
				}
			}
			$this->achievements = $temp;
			
			unset($key, $achievementData, $achievementInfo, $temp);
		}
		else
		{
			$this->achievements = false;
		}
		
		unset($charRecentAchievements);
	}
	
	private function getIcon($id)
	{
		if (isset($this->runtimeCache['icons'][$id]))
		{
			return $this->runtimeCache['icons'][$id];
		}
		
		$DisplayIdCache = $this->cache->get("items/item_displayid_".$this->realm."_".$id);

		if ($DisplayIdCache !== false)
		{
			$IconCache = $this->cache->get("items/display_".$DisplayIdCache);

			if ($IconCache != false)
			{
				$this->runtimeCache['icons'][$id] = $IconCache;
				return $IconCache;
			}
			else
			{
				$this->runtimeCache['icons'][$id] = "inv_misc_questionmark";
				return "inv_misc_questionmark";
			}
		}
		else
		{
			return false;
		}
	}
	
	private function GetItemEnchant($enchantments)
	{
		if ($this->getEmulatorString() == 'arcemu')
		{
			return $this->GetItemEnchant_Arcemu($enchantments);
		}
		
		if ($this->getEmulatorString() == 'mangos')
		{
			return $this->GetItemEnchant_Mangos($enchantments);
		}
		
		//explode the item enchantments
		$enchantments = explode(' ', $enchantments);
		
		//make a little loop
		for ($i = 0; $i <= 5; $i++)
		{
			if ($enchantments[$i] != 0)
			{
				//return the first found enchant
				return $this->getDbcModel()->getEnchantmentInfo($enchantments[$i]);
			}
		}
		
		//as default no enchant
		return false;
	}
	
	private function GetItemEnchant_Arcemu($enchantments)
	{
		if ($enchantments != '')
		{
			//remove the last delimeter
			$enchantments = rtrim($enchantments, ';');
			
			//split the data
			$data = explode(';', $enchantments);
			
			//loop trough the enchants
			foreach ($data as $string)
			{
				list($EnchantEntry, $duration, $slot) = explode(',', $string);
				//item enchants are at slot 0
				if ($slot == 0)
				{
					return $this->getDbcModel()->getEnchantmentInfo($EnchantEntry);
				}
			}
			unset($data, $enchantments);
		}
		
		//as default no enchant
		return false;
	}
	
	private function GetItemEnchant_Mangos($enchantments)
	{
		//explode the item enchantments
		$enchantments = explode(' ', $enchantments);
		
		if ((int)$enchantments[22] > 0)
		{
			if ($EnchInfo = $this->getDbcModel()->getEnchantmentInfo((int)$enchantments[22]))
			{
				return $EnchInfo;
			}
		}
		
		//as default no enchant
		return false;
	}
	
	private function hasExtraSocket($enchantments)
	{
		if ($this->getEmulatorString() == 'arcemu')
		{
			return $this->hasExtraSocket_Arcemu($enchantments);
		}
		
		if ($this->getEmulatorString() == 'mangos')
		{
			return $this->hasExtraSocket_Mangos($enchantments);
		}
		
		//explode the item enchantments
		$enchantments = explode(' ', $enchantments);
		
		if ((int)$enchantments[18] > 0)
		{
			switch ((int)$enchantments[18])
			{
				case 3319:
				case 3717:
				case 3723:
				case 3729:
				case 3848:
					return true;
				default:
					return false;
			}
		}
		
		return false;
	}
	
	private function hasExtraSocket_Arcemu($enchantments)
	{
		if ($enchantments != '')
		{
			//remove the last delimeter
			$enchantments = rtrim($enchantments, ';');
			
			//split the data
			$data = explode(';', $enchantments);
			
			//loop trough the enchants
			foreach ($data as $string)
			{
				list($EnchantEntry, $duration, $slot) = explode(',', $string);
				//extra socket enchants
				switch ((int)$EnchantEntry)
				{
					case 3319:
					case 3717:
					case 3723:
					case 3729:
					case 3848:
						return true;
				}
			}
			unset($data, $enchantments);
		}
		
		//as default no enchant
		return false;
	}
	
	private function hasExtraSocket_Mangos($enchantments)
	{
		//explode the item enchantments
		$enchantments = explode(' ', $enchantments);
		
		//Get the enchant, always position 40
		$Enchant = (int)$enchantments[40];
		
		if ($Enchant > 0)
		{
			switch ($Enchant)
			{
				case 3319:
				case 3717:
				case 3723:
				case 3729:
				case 3848:
					return true;
				default:
					return false;
			}
		}
		
		return false;
	}
	
	private function GetItemGems($enchantments)
	{
		if ($this->getEmulatorString() == 'arcemu')
		{
			return $this->GetItemGems_Arcemu($enchantments);
		}
		
		if ($this->getEmulatorString() == 'mangos')
		{
			return $this->GetItemGems_Mangos($enchantments);
		}
		
		//explode the item enchantments
		$enchantments = explode(' ', $enchantments);
		
		//temp array
		$temp = array();
		
		//make a little loop, this should get max of 3 standart gems
		for ($i = 6; $i <= 14; $i++)
		{
			if ($enchantments[$i] != 0)
			{
				$info = $this->getDbcModel()->getEnchantmentInfo($enchantments[$i]);
				
				//verify that this is a gem and not a gem bonus
				if ((int)$info['GemID'] > 0)
				{
					//if meta, get conditions data
					if ((int)$info['color'] == 1 && (int)$info['EnchantmentCondition'] > 0)
					{
						//By default we have no required gems
						$requiries = $this->getMetaRequiries($info['EnchantmentCondition']);
					}
					
					//Try getting the icon
					if ((int)$info['GemID'] > 0)
					{
						if (!($icon = $this->getIcon((int)$info['GemID'])))
						{
							$icon = false;
						}
					}
					else
					{
						$icon = false;
					}
					
					//Set the gem slot
					switch ($i)
					{
						case 6: $slot = 0; break;
						case 9: $slot = 1; break;
						case 12: $slot = 2; break;
					}
					
					//add to the temp array
					$temp[] = array(
						'GemID'		=> (int)$info['GemID'],
						'icon'		=> $icon,
						'color'		=> (int)$info['color'],
						'requires'	=> (isset($requiries) ? $requiries : false),
						'slot'		=> $slot,
						'text'		=> $info['description']
					);
					
					unset($icon, $info);
				}
			}
		}
		
		if (!empty($temp))
			return $temp;
		
		//as default no gems
		return false;
	}
	
	private function GemColorFix($color)
	{
		switch ($color)
		{
			case 3: return 4;
			case 4: return 8;
		}
		
		return $color;
	}
	
	private function GetItemGems_Arcemu($enchantments)
	{
		if ($enchantments != '')
		{
			//remove the last delimeter
			$enchantments = rtrim($enchantments, ';');
			
			//split the data
			$data = explode(';', $enchantments);
			
			//temp array
			$temp = array();
			
			//loop trough the enchants
			foreach ($data as $string)
			{
				list($EnchantEntry, $duration, $slot) = explode(',', $string);
				
				$info = $this->getDbcModel()->getEnchantmentInfo($EnchantEntry);
				
				//verify that this is a gem and not a gem bonus
				if ((int)$info['GemID'] > 0)
				{
					//if meta, get conditions data
					if ((int)$info['color'] == 1 && (int)$info['EnchantmentCondition'] > 0)
					{
						//By default we have no required gems
						$requiries = $this->getMetaRequiries($info['EnchantmentCondition']);
					}
					
					//Try getting the icon
					if (!($icon = $this->getIcon((int)$info['GemID'])))
					{
						$icon = false;
					}
					
					//Set the gem slot
					switch ($slot)
					{
						case 2: $slot = 0; break;
						case 3: $slot = 1; break;
						case 4: $slot = 2; break;
					}
					
					//add to the temp array
					$temp[] = array(
						'GemID'		=> (int)$info['GemID'],
						'color'		=> (int)$info['color'],
						'icon'		=> $icon,
						'requires'	=> (isset($requiries) ? $requiries : false),
						'slot'		=> $slot,
						'text'		=> $info['description']
					);
				}
			}
			unset($data, $enchantments);
			
			if (!empty($temp))
				return $temp;
		}
		
		//as default no enchant
		return false;
	}
	
	private function GetItemGems_Mangos($enchantments)
	{
		//split the data
		$data = explode(' ', $enchantments);
		
		//temp array
		$temp = array();
		
		//At these indexes are the gems stored
		$Positions = array(28, 31, 34);
		
		foreach ($Positions as $index)
		{
			//Get the gem
			$EnchID = (int)$data[$index];
			
			if ($EnchID > 0)
			{
				$info = $this->getDbcModel()->getEnchantmentInfo($EnchID);
				
				//verify that this is a gem and not a gem bonus
				if ((int)$info['GemID'] > 0)
				{
					//if meta, get conditions data
					if ((int)$info['color'] == 1 && (int)$info['EnchantmentCondition'] > 0)
					{
						//By default we have no required gems
						$requiries = $this->getMetaRequiries($info['EnchantmentCondition']);
					}
					
					//Try getting the icon
					if (!($icon = $this->getIcon((int)$info['GemID'])))
					{
						$icon = false;
					}
					
					//Set the gem slot
					switch ($index)
					{
						case 28: $slot = 0; break;
						case 31: $slot = 1; break;
						case 34: $slot = 2; break;
					}
					
					//add to the temp array
					$temp[] = array(
						'GemID'		=> (int)$info['GemID'],
						'color'		=> (int)$info['color'],
						'icon'		=> $icon,
						'requires'	=> (isset($requiries) ? $requiries : false),
						'slot'		=> $slot,
						'text'		=> $info['description']
					);
				}
			}
		}
		unset($data, $enchantments);
		
		if (!empty($temp))
			return $temp;
		
		//as default no enchant
		return false;
	}
	
	private function getMetaRequiries($ConditionId)
	{
		//By default we have no required gems
		$requiries = false;
		
		//Get the condition data
		$condition = $this->getDbcModel()->getEnchantmentConditions((int)$ConditionId);
		
		//Check what gems we require
		for ($i = 1; $i <= 5; $i++)
		{
			if (((int)$condition['Color' . $i] > 0 && (int)$condition['Value' . $i] > 0) || ((int)$condition['Color' . $i] > 0 && (int)$condition['CompareColor' . $i] > 0))
			{
				$requiries[] = array(
					'color' => $this->GemColorFix($condition['Color' . $i]), 
					'count' => $condition['Value' . $i], 
					'comparator' => $condition['Comparator' . $i], 
					'compareColor' => $this->GemColorFix($condition['CompareColor' . $i])
				);
			}
		}
		
		unset($condition);
		
		return $requiries;
	}
	
	private function getBackground()
	{
		switch($this->raceName)
		{
			default: return "shattrath"; break;
			case "Human": return "stormwind"; break;
			case "Blood elf": return "silvermoon"; break;
			case "Night elf": return "darnassus"; break;
			case "Dwarf": return "ironforge"; break;
			case "Gnome": return "ironforge"; break;
			case "Orc": return "orgrimmar"; break;
			case "Draenei": return "theexodar"; break;
			case "Tauren": return "thunderbluff"; break;
			case "Undead": return "undercity"; break;
			case "Troll": return "orgrimmar"; break;
		}
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
	
	private function getProfessionsInfo()
	{
		//Get the character's professions
		$professions = $this->armory_model->getProfessions();
		
		//Prevent undefined notice
		$this->professions['secondary'] = false;
		
		if ($professions)
		{
			//get the main professions
			foreach ($professions as $key => $row)
			{
				//calculate the percentages
				$row['percent'] = $this->percent($row['value'], $row['max']);
				
				if ($row['category'] === 0)
				{
					$this->professions['main'][] = $row;
				}
				else if ($row['category'] === 1)
				{
					$this->professions['secondary'][] = $row;
				}
			}
		}
		
		//make sure we have two records for them main profs
		for ($i = 0; $i < 2; $i++)
		{
			if (!isset($this->professions['main'][$i]))
				$this->professions['main'][$i] = false;
		}
		
		unset($professions);
	}
	
	public function percent($num_amount, $num_total)
	{
		$count1 = $num_amount / $num_total;
		$count2 = $count1 * 100;
		$count = number_format($count2, 0);
		
		return $count;
	}
	
	/**
	 * Load the profile
	 * @return String
	 */
	private function getProfile()
	{
		$cache = $this->cache->get("character_" . ($this->realm ? $this->realm : 0) . "_" . $this->id."_".getLang());
		
		if ($cache !== false)
		{
			$this->template->setTitle($cache['name']);
			$this->template->setDescription($cache['description']);
			$this->template->setKeywords($cache['keywords']);
			
			$page = $cache['page'];
		}
		else
		{
			if($this->armory_model->characterExists())
			{
				// Load all items and info
				$this->getInfo();
				// Load the info about the talents
				$this->getTalentInfo();
				// Load professions
				$this->getProfessionsInfo();
				
				$this->template->setTitle($this->name);

				$avatarArray = array(
							'class' => $this->class,
							'race' => $this->race,
							'level' => $this->level,
							'gender' => $this->gender
						);
				
				$charData = array(
					"name" => $this->name,
					"race" => $this->race,
					"faction" => $this->getFactionName($this->class),
					"class" => $this->class,
					"level" => $this->level,
					"gender" => $this->gender,
					"items" => $this->items,
					"guild" => $this->guild,
					"guildName" => $this->guildName,
					"pvp" => $this->pvp,
					"url" => $this->template->page_url,
					"raceName" => $this->raceName,
					"className" => $this->className,
					"className_clean" => str_replace(' ', '', strtolower($this->className)),
					"realmName" => $this->realmName,
					"avatar" => $this->armory_model->realms->formatAvatarPath($avatarArray),
					"stats" => $this->stats,
					"secondBar" => $this->secondBar,
					"secondBarValue" => $this->secondBarValue,
					"bg" => $this->getBackground(),
					"realmId" => $this->realm,
					"fcms_tooltip" => $this->config->item("use_fcms_tooltip"),
					"has_stats" => $this->realms->getRealm($this->realm)->getEmulator()->hasStats(),
					//Talent & Glyph tables
					"talent_active_spec" => $this->getActiveSpec(),
					"talent_tables" => $this->getTalentTables(),
					"talent_specs" => $this->getSpecsTable(),
					"glyph_tables" => $this->getGlyphTables(),
					//achievements
					"recent_achievements" => $this->achievements,
					//professions
					"main_professions" => $this->professions['main'],
					"secondary_professions" => $this->professions['secondary'],
					//Get the arena teams
					"arena_teams_table" => $this->getArenaTeamsTable(),
					//Define the expansion to use for design crap
					"expansion_str" => ($this->isCataclysm() ? 'cata' : 'wotlk'),
					//Handle enchantments by this really strange way but i find it best
					"TooltipPlayerData"	=> (!empty($this->TooltipPlayerData) ? $this->TooltipPlayerData : false),
					"equippedItems" => (!empty($this->equippedItems) ? $this->equippedItems : false)
				);

				$character = $this->template->loadPage("character.tpl", $charData);

				$data = array(
					"module" => "default", 
					"headline" => "<span style='cursor:pointer;' data-tip='".lang("view_profile", "character")."' onClick='window.location=\"".$this->template->page_url."profile/".$this->account."\"'>".$this->accountName."</span> &rarr; ".$this->name,
					"content" => $character
				);

				$page = $this->template->loadPage("page.tpl", $data);
				
				$keywords = "armory,".$charData['name'].",lv".$charData['level'].",".$charData['raceName'].",".$charData['className'].",".$charData['realmName'];
				$description = $charData['name']." - level ".$charData['level']." " .$charData['raceName']." ".$charData['className']." on ".$charData['realmName'];
				
				$this->template->setDescription($description);
				$this->template->setKeywords($keywords);
				
				if($this->canCache)
				{
					// Cache for 30 min
					$this->cache->save("character_" . ($this->realm ? $this->realm : 0) . "_" . $this->id."_".getLang(), array('page' => $page, 'name' => $this->name, 'keywords' => $keywords, 'description' => $description), 60*30);
				}
			}
			else
			{
				$page = $this->getError(true);
			}
		}

		$this->template->view($page, $this->css, $this->js);
	}

	/**
	 * Show "character doesn't exist" error
	 */
	private function getError($get = false)
	{
		$this->template->setTitle("Character not found");

		$data = array(
			"module" => "default", 
			"headline" => lang("doesnt_exist", "character"), 
			"content" => "<center style='margin:10px;font-weight:bold;'>".lang("doesnt_exist_long", "character")."</center>"
		);

		$page = $this->template->loadPage("page.tpl", $data);

		if($get)
		{
			return $page;
		}
		else
		{
			$this->template->view($page);
		}
	}
	
	private function getDbcModel()
	{
		if ($this->isCataclysm())
		{
			$this->cata_dbc_model->setBuild($this->getEmulatorBuild());
			return $this->cata_dbc_model;
		}
		
		//default
		//wotlk 3.5.5a
		return $this->wotlk_dbc_model;
	}
	
	private function getTalentInfo()
	{
		$tClass = false;
		
		switch ($this->class)
		{
			case 1: $tClass = 1; break;
			case 2: $tClass = 2; break;
			case 3: $tClass = 4; break;
			case 4: $tClass = 8; break;
			case 5: $tClass = 16; break;
			case 6: $tClass = 32; break;
			case 7: $tClass = 64; break;
			case 8: $tClass = 128; break;
			case 9: $tClass = 256; break;
			case 11: $tClass = 1024; break;
		}
		
		//Get the character's talent specs info
		$this->talentSpecsInfo = $this->armory_model->getTalentSpecsInfo();
		
		//loop the specs 
		for ($spec = 0; $spec < $this->talentSpecsInfo['speccount']; $spec++)
		{
			//Get the character talents for the spec
			$this->talents[$spec] = $this->armory_model->getTalents($spec);
			//Get the glyphs for the spec
			$this->glyphs[$spec] = $this->getCharGlyphsTable($spec);
		}
		
		//Get the talent tree info
		$this->talentTabs = $this->getDbcModel()->getTalentTabs($tClass);
		
		//Get the base talents for each talent tree
		if (!empty($this->talentTabs))
		{
			foreach ($this->talentTabs as $key => $tab)
			{
				$this->talentTabs[$key]['talents'] = $this->getDbcModel()->getTalentsForTab($tab['id']);
			}
		}
		
		//Mangos and ArcEmu store the talents by talent_id unlike trinity which stores by spell
		//Convert to spell
		if ($this->getEmulatorString() == 'mangos' || $this->getEmulatorString() == 'mangosr2' || $this->getEmulatorString() == 'arcemu')
		{
			for ($spec = 0; $spec < $this->talentSpecsInfo['speccount']; $spec++)
			{
				if (isset($this->talents[$spec]) && $this->talents[$spec])
				{
					//loop the talents
					foreach ($this->talents[$spec] as $key => $row)
					{
						$this->talents[$spec][$key] = $this->getTalentSpellByIdRank($row['talent_id'], $row['current_rank']);
					}
				}
			}
		}
		
		$this->getSpecsTable();
		
		unset($tClass);
	}
	
	//required for mangos and arcemu
	private function getTalentSpellByIdRank($id, $rank)
	{
		//increase the rank number for the column name
		$rank = $rank + 1;
		
		if (!empty($this->talentTabs))
		{
			foreach ($this->talentTabs as $key => $tab)
			{
				foreach ($this->talentTabs[$key]['talents'] as $k => $talent)
				{
					//if found
					if ($talent['id'] == $id)
					{
						return $talent['rank'.$rank];
					}
				}
			}
		}
		
		return false;
	}
	
	private function getActiveSpec()
	{
		return $this->talentSpecsInfo['activespec'];
	}
	
	private function getSpecsTable()
	{
		$table = false;
		
		//loop the specs 
		for ($spec = 0; $spec < $this->talentSpecsInfo['speccount']; $spec++)
		{
			//defaults
			$table[$spec] = array(
				'title'		=> 'Undetermined',
				'icon'		=> false,
				'points'	=> '0/0/0',
				'active'	=> ($spec == $this->talentSpecsInfo['activespec'] ? true : false),
				'mainTree'	=> false
			);
			$tabPoints = array();
			
			//loop the tabs and determine the points spend
			foreach ($this->talentTabs as $key => $tab)
			{
				$tabPoints[$tab['order']] = 0;
				
				foreach ($tab['talents'] as $talent)
				{
					$tabPoints[$tab['order']] = $tabPoints[$tab['order']] + $this->getPointsSpendOnTalent($talent, $spec);
				}
			}
			
			//determine the spec title
			//Check if any points are spend
			if ($tabPoints[0] > 0 || $tabPoints[1] > 0 ||$tabPoints[2] > 0)
			{
				//set the points string for the spec
				$table[$spec]['points'] = implode('/', $tabPoints);
				
				$temp = $tabPoints;
				arsort($temp);
				$mainTree = key($temp);
				unset($temp);
				
				$table[$spec]['mainTree'] = $mainTree;
				
				//find the main tab
				foreach ($this->talentTabs as $key => $tab)
				{
					if ($tab['order'] == $mainTree)
					{
						$table[$spec]['title'] = $tab['name'];
						$table[$spec]['icon'] = strtolower($tab['icon']);
						//break the loop
						break;
					}
				}
			}
			
			unset($tabPoints);
		}
		
		return $table;
	}
	
	private function getPointsSpendOnTalent($talent, $spec)
	{
		$points = 0;
		
		if (!$this->talents[$spec])
			return 0;
		
		//loop trough the talent ranks and check if the character has spend any points
		for ($i = 1; $i <= 5; $i++)
		{
			//check if the talent has this rank
			if ($talent['rank'.$i] != '0')
			{
				$search = array_search($talent['rank'.$i], $this->talents[$spec]);
				if ($search !== false)
				{
					$points = $i;
				}
				unset($search);
			}
			else
				break;
		}
		
		return $points;
	}
	
	private function getTalentTable($specId = 0)
	{
		$table = false;
		
		if (!empty($this->talentTabs))
		{
			$table = array();
			
			foreach ($this->talentTabs as $key => $tab)
			{
				//save some tab info
				$table[$key]['id'] = $tab['id'];
				$table[$key]['order'] = $tab['order'];
				$table[$key]['name'] = $tab['name'];
				$table[$key]['icon'] = strtolower($tab['icon']);
				//Now we create the talent table
				$table[$key]['table'] = array();
				//WOTLK
				//every talent tree has 11 rows
				//each row has 4 columns
				//CATA
				//every talent tree has 7 rows
				//each row has 4 columns
				$rowsToLoop = ($this->isCataclysm() ? 6 : 10);
				//let's dance tango
				for ($row = 0; $row <= $rowsToLoop; $row++)
				{
					//loop trough the columns
					for ($col = 0; $col <= 3; $col++)
					{
						//column has no talent by default
						if (!isset($table[$key]['table'][$row][$col]))
							$table[$key]['table'][$row][$col] = false;
						
						//check if this column has talent
						if ($talent = $this->findTalentRow($tab, $row, $col))
						{
							//On patch 4.0.6a there are multiple records for the same position
							//try fixing the invalid records
							//check if the current talent row is valid by trying to get the talent icon
							if (!$this->getSpellIcon($talent['rank1']))
							{
								//erase the record for this talent
								$tab = $this->eraseTabTalent($talent, $tab);
								//get the next record
								$talent = $this->findTalentRow($tab, $row, $col);
								//verify it
								if (!$talent)
									continue;
							}
							
							//Set the character data for this talent
							$charTalentData = $this->findCharacterTalentData($talent, $specId);
							
							//First check if we have arrows preset
							if ($table[$key]['table'][$row][$col])
								$charTalentData['arrows'] = $table[$key]['table'][$row][$col]['arrows'];
							
							$table[$key]['table'][$row][$col] = $charTalentData;
							
							unset($charTalentData);
							
							//Get the spell icon
							$table[$key]['table'][$row][$col]['icon'] = $this->getSpellIcon($talent['rank1']);
							
							//Check if this talent is dependant
							if ((int)$talent['dependsOn'] > 0)
							{
								if ($dependency = $this->findTalentDependency($talent, $key))
								{
									//arrow defaults
									$arrowData = array('pointing' => '');
									
									//Let's find out how to place the dependency arrow
									//first determine if there is need for horizontal arrow
									if ($dependency['col'] != $talent['col'])
									{
										//determine if it's pointing left
										if ($dependency['col'] > $talent['col'])
											$arrowData['pointing'] = 'left';
										else
											$arrowData['pointing'] = 'right';
										
										//determine if we should point down aswell
										if ($dependency['row'] != $talent['row'])
										{
											$arrowData['pointing'] = $arrowData['pointing'] . 'down';
											//determine how many rows down we should go
											$arrowData['rows'] = (($talent['row'] + 1) - ($dependency['row'] + 1)) - 1;
										}
									}
									else
									//It's only vertically pointing
									{
										$arrowData['pointing'] = 'down';
										//determine how many rows down we should go
										$arrowData['rows'] = (($talent['row'] + 1) - ($dependency['row'] + 1)) - 1;
									}
									
									//save the arrow data
									$table[$key]['table'][$dependency['row']][$dependency['col']]['arrows'][] = $arrowData;
									
									unset($arrowData);
								}
								unset($dependency);
							}
						}
						unset($talent);
					}
					unset($col);
				}
				unset($row, $rowsToLoop);
			}
			unset($key, $tab);
		}
		
		return $table;
	}
	
	private function getTalentTables()
	{
		$table = false;
		
		//loop the specs 
		for ($spec = 0; $spec < $this->talentSpecsInfo['speccount']; $spec++)
		{
			$table[$spec] = $this->getTalentTable($spec);
		}
		
		return $table;
	}
	
	private function getCharGlyphsTable($spec)
	{
		//Get some info about the glyphs and convert to table
		$charGlyphsData = $this->armory_model->getGlyphs($spec);
		
		//handle glyph records for diferrent emulators
		switch ($this->getEmulatorString())
		{
			case 'trinity':
			case 'skyfire':
			case 'arkcore':
			case 'trinity_cata':
				return $this->getGlyphsTableTrinity($charGlyphsData[0]);
			case 'mangos':
			case 'mangosr2':
			case 'arcemu':
				return $this->getGlyphsTableMangos($charGlyphsData);
			default:
				return false;
		}
	}
	
	//TrinityCore and Skyfire
	private function getGlyphsTableTrinity($data)
	{
		//determine the glyphs count
		$glyphs = (isset($data['glyph9']) ? 9 : 6);
			
		$temp = array();
		//let's make it an array with record for each glyph
		for ($i = 1; $i <= $glyphs; $i++)
		{
			$glyphId = $data['glyph'.$i];
			
			if ((int)$glyphId > 0)
				$temp[] = $this->getDbcModel()->getGlyphInfo($glyphId);
			else
				$temp[] = false;
		}
		unset($i, $glyphs, $glyphId, $data);
		
		return $temp;
	}
	
	//Mangos
	private function getGlyphsTableMangos($data)
	{
		if (!$data || empty($data))
			return false;
		
		//let's make it an array with record for each glyph
		foreach ($data as $key => $row)
		{
			if ((int)$row['glyph'] > 0)
				$temp[] = $this->getDbcModel()->getGlyphInfo($row['glyph']);
			else
				$temp[] = false;
		}
		unset($data, $key, $row);
		
		return $temp;
	}
	
	private function getGlyphTable($spec)
	{
		$table = array(
			'minor'		=> array(),
			'major'		=> array(),
			'hasPrime'	=> false,
			'prime'		=> array(),
		);
		
		if (isset($this->glyphs[$spec]) && $this->glyphs[$spec] !== false)
		{
			$table['hasPrime'] = ($this->isCataclysm() ? true : false);
			
			//Loop the glyphs to do some cosmetics
			foreach ($this->glyphs[$spec] as $key => $glyph)
			{
				if ($glyph)
				{
					//flag 2 = prime
					//flag 1 = minor
					//flag 0 = major
					if (($glyph['typeflags'] & 2) == 2)
					{
						$glyph['icon'] = 'inv_glyph_prime'.str_replace(' ', '', strtolower($this->className));
						//push to the table
						$table['prime'][] = $glyph;
					}
					else if (($glyph['typeflags'] & 1) == 1)
					{
						$glyph['icon'] = 'inv_glyph_minor'.str_replace(' ', '', strtolower($this->className));
						//push to the table
						$table['minor'][] = $glyph;
					}
					else
					{
						$glyph['icon'] = 'inv_glyph_major'.str_replace(' ', '', strtolower($this->className));
						//push to the table
						$table['major'][] = $glyph;
					}
				}
			}
		}
		
		//fill in the gaps
		while (count($table['minor']) < 3)
			$table['minor'][] = array('id' => 0, 'spellid' => 0, 'name' => 'Empty', 'icon' => 'inventoryslot_empty');
			
		while (count($table['major']) < 3)
			$table['major'][] = array('id' => 0, 'spellid' => 0, 'name' => 'Empty', 'icon' => 'inventoryslot_empty');
			
		while (count($table['prime']) < 3)
			$table['prime'][] = array('id' => 0, 'spellid' => 0, 'name' => 'Empty', 'icon' => 'inventoryslot_empty');
		
		return $table;
	}
	
	private function getGlyphTables()
	{
		$table = false;
		
		//loop the specs 
		for ($spec = 0; $spec < $this->talentSpecsInfo['speccount']; $spec++)
		{
			$table[$spec] = $this->getGlyphTable($spec);
		}
		
		return $table;
	}
	
	private function findTalentDependency($talentData, $tabId)
	{
		if (isset($this->talentTabs[$tabId]))
		{
			foreach ($this->talentTabs[$tabId]['talents'] as $talent)
			{
				if ($talent['id'] == $talentData['dependsOn'])
				{
					return $talent;
				}
			}
		}
		
		return false;
	}
	
	private function findTalentRow($tab, $row, $col)
	{
		foreach ($tab['talents'] as $key => $talent)
		{
			if ($talent['row'] == $row)
			{
				if ($talent['col'] == $col)
				{
					return $talent;
				}
			}
		}
		
		return false;
	}
	
	private function eraseTabTalent($rTalent, $tab)
	{
		$new = array();
		
		foreach ($tab['talents'] as $key => $talent)
		{
			if ($talent['rank1'] == $rTalent['rank1'] && $talent['rank2'] == $rTalent['rank2'])
			{
				//this is the invalid talent, skip it
				continue;
			}
			
			//push to the new table
			$new[] = $talent;
		}
		
		$tab['talents'] = $new;
		
		return $tab;
	}
	
	private function findCharacterTalentData($talent, $spec)
	{
		//By default we return the first talent rank with zero points spend
		$return = array(
			'spell'		=> $talent['rank1'],
			'points'	=> 0,
			'max_rank'	=> 0
		);
		
		//loop trough the talent ranks and check if the character has spend any points
		for ($i = 1; $i <= 5; $i++)
		{
			//check if the talent has this rank
			if ($talent['rank'.$i] != '0')
			{
				if (isset($this->talents[$spec]) && $this->talents[$spec])
				{
					$search = array_search($talent['rank'.$i], $this->talents[$spec]);
					if ($search !== false)
					{
						$return['spell'] = $talent['rank'.$i];
						$return['points'] = $i;
					}
					unset($search);
				}
				
				$return['max_rank'] = $i;
			}
			else
				break;
			}
		
		return $return;
	}
	
	private function getSpellIcon($spell)
	{
		if ($icon = $this->getDbcModel()->getSpellIcon($spell))
			return str_replace(' ', '-', $icon['icon']);
			
		return false;
	}
	
	private function getArenaTeamsTable()
	{
		return array(
				0 => $this->armory_model->getTeam(2),
				1 => $this->armory_model->getTeam(3),
				2 => $this->armory_model->getTeam(5)
			);
	}
}
