<?php

class Tooltip extends MX_Controller
{
	private $id;
	private $realm;
	private $item;
	private $flags;
	private $extraFlags;
	private $additionalData;
	private $EmulatorSimpleString = '';

	public function Index($realm = false, $id = false)
	{
		// Make sure item and realm are set
		if (!$id || !$realm)
		{
			die("No item or realm specified!");
		}

		$cache = $this->cache->get("items/tooltip_".$realm."_".$id."_".getLang());
		
		if ($cache !== false)
		{
			die($cache);
		}
		else
		{
			// Assign values
			$this->id = $id;
			$this->realm = $realm;
			$this->EmulatorSimpleString = str_replace(array('_ra', '_soap', '_rbac'), '', $this->realms->getRealm($this->realm)->getConfig('emulator'));
			
			$this->getItemData();

			$data = array(
					'module' => 'tooltip',
					'item' => $this->item
				);

			$out = $this->template->loadPage("tooltip.tpl", $data);

			// Cache it
			$this->cache->save("items/tooltip_".$realm."_".$id."_".getLang(), $out);

			die($out);
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
	 * Gather all data needed
	 */
	private function getItemData()
	{
		// Load constants
		$this->load->config("tooltip_constants");

		// Assign them
		$bind = $this->config->item("bind");
		$slots = $this->config->item("slots");
		$damages = $this->config->item("damages");

		// Load the realm
		$realmObj = $this->realms->getRealm($this->realm);

		// Get the item SQL data
		$item = $this->getItem($this->id);
		
		// No item was found
		if (!$item || $item == "empty")
		{
			die(lang("unknown_item", "item"));
		}
		
		// Set the additional data
		$this->additionalData = array('itemset' => $item['itemset'], 'socketBonus' => $item['socketBonus']);
		
		// Set the flags
		$this->flags = $this->getFlags($item['Flags']);
		
		if (isset($item['FlagsExtra']))
			$this->extraFlags = $this->getFlags($item['FlagsExtra']);

		$this->item['name'] = $item['name'];
		
		// Support custom colors
		if(preg_match("/\|cff/", $item['name']))
		{
			while(preg_match("/\|cff/", $this->item['name']))
			{
				$this->item['name'] = preg_replace("/\|cff([A-Za-z0-9]{6})(.*)(\|cff)?/", '<span style="color:#$1;">$2</span>', $this->item['name']);
			}
		}
		
		$this->item['quality'] = $item['Quality'];
		$this->item['bind'] = $bind[$item['bonding']];
		$this->item['unique'] = ($this->hasFlag(524288)) ? "Unique-Equipped" : null;
		$this->item['isHeroic'] = ($this->hasFlag(8)) ? true : false;
		$this->item['slot'] = $slots[$item['InventoryType']];
		$this->item['durability'] = $item['MaxDurability'];
		$this->item['required'] = $item['RequiredLevel'];
		$this->item['level'] = $item['ItemLevel'];
		$this->item['type'] = $this->getType($item['class'], $item['subclass']);
		$this->item['damage_type'] = (array_key_exists("dmg_type1", $item)) ? $damages[$item['dmg_type1']] : false;
		
		if(array_key_exists("dmg_min1", $item))
		{
			$this->item['damage_min'] = $item['dmg_min1'];
			$this->item['damage_max'] = $item['dmg_max1'];
		}
		// For SkyFire: calculate weapon damage manually
		else if($item['class'] == 2)
		{
			//override stat scaling faction, skyfire has all values 0
			if (!isset($item['StatScalingFactor']) || (int)$item['StatScalingFactor'] == 0)
				$item['StatScalingFactor'] = 1;
			
			if ($dmg = $this->getItemDamage($item['ItemLevel'], $item['class'], $item['subclass'], $item['Quality'], $item['delay'], $item['StatScalingFactor'], $item['InventoryType']))
			{
				$this->item['damage_min'] = $dmg['damage_min'];
				$this->item['damage_max'] = $dmg['damage_max'];
				$this->item['dps'] = $dmg['dps'];
			}
			else
			{
				$this->item['damage_min'] = false;
				$this->item['damage_max'] = false;
			}
		}
		else
		{
			$this->item['damage_min'] = false;
			$this->item['damage_max'] = false;
		}
		
		if (array_key_exists("armor", $item))
		{
			$this->item['armor'] = $item['armor'];
		}
		//Handle armor for skyfire
		else if ($item['class'] == 4)
		{
			if ($armor = $this->getItemArmor($item['ItemLevel'], $item['class'], $item['subclass'], $item['Quality'], $item['InventoryType']))
			{
				$this->item['armor'] = $armor;
			}
			else
				$this->item['armor'] = false;
		}
		else
		{
			$this->item['armor'] = false;
		}
			
		$this->item['spells'] = $this->getSpells($item);
		$this->item['attributes'] = $this->getAttributes($item);
		$this->item['holy_res'] = (array_key_exists("holy_res", $item)) ? $item['holy_res'] :  $this->getAttributeById(53, $item);
		$this->item['fire_res'] = (array_key_exists("fire_res", $item)) ? $item['fire_res'] :  $this->getAttributeById(51, $item);
		$this->item['nature_res'] = (array_key_exists("nature_res", $item)) ? $item['nature_res'] :  $this->getAttributeById(55, $item);
		$this->item['frost_res'] = (array_key_exists("frost_res", $item)) ? $item['frost_res'] :  $this->getAttributeById(52, $item);
		$this->item['shadow_res'] = (array_key_exists("shadow_res", $item)) ? $item['shadow_res'] :  $this->getAttributeById(54, $item);
		$this->item['arcane_res'] = (array_key_exists("arcane_res", $item)) ? $item['arcane_res'] :  $this->getAttributeById(56, $item);
		$this->item['speed'] = ($item['delay'] > 0) ? ($item['delay'] / 1000) . "0": 0;
		
		if (!isset($this->item['dps']))
			$this->item['dps'] = $this->getDPS($this->item['damage_min'], $this->item['damage_max'], $this->item['speed']);
			
		$this->item['sockets'] = $this->getSockets($item);
		$this->item['socketBonus'] = $this->getSocketBonus();
		$this->item['itemSet'] = $this->getItemSet();
	}
	
	private function getItem($entry)
	{
		$ItemCache = $this->cache->get("items/item_".$this->realm."_".$entry);
		
		if ($ItemCache && $ItemCache != 'empty')
		{
			//Check if the cached copy includes our extra fields
			if (isset($ItemCache['socketBonus']))
			{
				return $ItemCache;
			}
		}
		
		// Use new query in case we support the emulator
		switch ($this->getEmulatorString())
		{
			case 'trinity':
				$Statement = "	SELECT 
									entry, Flags, name, Quality, bonding, InventoryType, MaxDurability, armor, RequiredLevel, ItemLevel, class, subclass, 
									dmg_min1, dmg_max1, dmg_type1, holy_res, fire_res, nature_res, frost_res, shadow_res, arcane_res, delay, 
									socketColor_1, socketColor_2, socketColor_3, spellid_1, spellid_2, spellid_3, spellid_4, spellid_5, spelltrigger_1, spelltrigger_2, spelltrigger_3, spelltrigger_4, spelltrigger_5, 
									displayid, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3, stat_type4, stat_value4, stat_type5, stat_value5, 
									stat_type6, stat_value6, stat_type7, stat_value7, stat_type8, stat_value8, stat_type9, stat_value9, stat_type10, stat_value10, stackable, socketBonus, itemset 
								FROM item_template 
								WHERE entry=?";
				break;
			case 'trinity_cata':
				$Statement = "	SELECT 
									entry, Flags, name, Quality, bonding, InventoryType, MaxDurability, RequiredLevel, ItemLevel, class, subclass, delay, 
									socketColor_1, socketColor_2, socketColor_3, spellid_1, spellid_2, spellid_3, spellid_4, spellid_5, spelltrigger_1, spelltrigger_2, spelltrigger_3, spelltrigger_4, spelltrigger_5, 
									displayid, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3, stat_type4, stat_value4, stat_type5, stat_value5, 
									stat_type6, stat_value6, stat_type7, stat_value7, stat_type8, stat_value8, stat_type9, stat_value9, stat_type10, stat_value10, 
									stackable, socketBonus, itemset, StatScalingFactor, FlagsExtra 
								FROM item_template 
								WHERE entry=?";
				break;
			case 'skyfire':
				$Statement = "	SELECT 
									entry, Flags, name, Quality, bonding, InventoryType, MaxDurability, RequiredLevel, ItemLevel, class, subclass, delay, 
									socketColor_1, socketColor_2, socketColor_3, spellid_1, spellid_2, spellid_3, spellid_4, spellid_5, spelltrigger_1, spelltrigger_2, spelltrigger_3, spelltrigger_4, spelltrigger_5, 
									displayid, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3, stat_type4, stat_value4, stat_type5, stat_value5, 
									stat_type6, stat_value6, stat_type7, stat_value7, stat_type8, stat_value8, stat_type9, stat_value9, stat_type10, stat_value10, 
									stackable, socketBonus, itemset, StatScalingFactor, FlagsExtra 
								FROM item_template 
								WHERE entry=?";
				break;
			case 'arkcore':
				$Statement = "	SELECT 
									entry, Flags, name, Quality, bonding, InventoryType, MaxDurability, RequiredLevel, ItemLevel, class, subclass, delay, 
									socketColor_1, socketColor_2, socketColor_3, spellid_1, spellid_2, spellid_3, spellid_4, spellid_5, spelltrigger_1, spelltrigger_2, spelltrigger_3, spelltrigger_4, spelltrigger_5, 
									displayid, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3, stat_type4, stat_value4, stat_type5, stat_value5, 
									stat_type6, stat_value6, stat_type7, stat_value7, stat_type8, stat_value8, stat_type9, stat_value9, stat_type10, stat_value10, 
									stackable, socketBonus, itemset, ScalingStatValue AS StatScalingFactor, FlagsExtra 
								FROM item_template 
								WHERE entry=?";
				break;
			case 'mangos':
				$Statement = " 	SELECT 
									entry, Flags, name, Quality, bonding, InventoryType, MaxDurability, armor, RequiredLevel, ItemLevel, class, subclass, dmg_min1, dmg_max1, dmg_type1, 
									holy_res, fire_res, nature_res, frost_res, shadow_res, arcane_res, delay, 
									socketColor_1, socketColor_2, socketColor_3, spellid_1, spellid_2, spellid_3, spellid_4, spellid_5, spelltrigger_1, spelltrigger_2, spelltrigger_3, spelltrigger_4, spelltrigger_5, 
									displayid, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3, stat_type4, stat_value4, stat_type5, stat_value5, 
									stat_type6, stat_value6, stat_type7, stat_value7, stat_type8, stat_value8, stat_type9, stat_value9, stat_type10, stat_value10, stackable, socketBonus, itemset 
								FROM item_template 
								WHERE entry=?";
				break;
			case 'arcemu':
				$Statement = "	SELECT 
									entry entry, flags Flags, name1 name, quality Quality, bonding bonding, inventorytype InventoryType, MaxDurability MaxDurability, 
									armor armor, requiredLevel RequiredLevel, itemLevel ItemLevel, class class, subclass subclass, 
									dmg_min1 dmg_min1, dmg_max1 dmg_max1, dmg_type1 dmg_type1, holy_res holy_res, fire_res fire_res, nature_res nature_res, frost_res frost_res, shadow_res shadow_res, arcane_res arcane_res, 
									delay delay, socket_color_1 socketColor_1, socket_color_2 socketColor_2, socket_color_3 socketColor_3, 
									spellid_1 spellid_1, spellid_2 spellid_2, spellid_3 spellid_3, spellid_4 spellid_4, spellid_5 spellid_5, 
									spelltrigger_1 spelltrigger_1, spelltrigger_2 spelltrigger_2, spelltrigger_3 spelltrigger_3, spelltrigger_4 spelltrigger_4, spelltrigger_5 spelltrigger_5, 
									displayid displayid, stat_type1 stat_type1, stat_value1 stat_value1, stat_type2 stat_type2, stat_value2 stat_value2, stat_type3 stat_type3, stat_value3 stat_value3, 
									stat_type4 stat_type4, stat_value4 stat_value4, stat_type5 stat_type5, stat_value5 stat_value5, 
									stat_type6 stat_type6, stat_value6 stat_value6, stat_type7 stat_type7, stat_value7 stat_value7, 
									stat_type8 stat_type8, stat_value8 stat_value8, stat_type9 stat_type9, stat_value9 stat_value9, stat_type10 stat_type10, stat_value10 stat_value10, 
									socket_bonus AS socketBonus, itemset 
								FROM items 
								WHERE entry=?";
				break;
			default:
				return $this->realms->getRealm($this->realm)->getWorld()->getItem($this->id);
		}
		
		//Get the world database
		$DB = $this->realms->getRealm($this->realm)->getWorld()->getConnection();
		//Run the query
		$query = $DB->query($Statement, array($entry));
		
		if ($DB->_error_message())
		{
			die($DB->_error_message());
		}
		
		if ($query && $query->num_rows() > 0)
		{
			$result = $query->result_array();
			
			// Cache it forever
			$this->cache->save("items/item_".$this->realm."_".$entry, $result[0]);
			
			return $result[0];
		}
		//Check if we have no item data, TrinityCore Cata dont have them items in the DB
		else if ($this->isCataclysm())
		{
			//Pull the item data from the fusion database
			return $this->getItemDB2($entry);
		}
		
		return false;
	}
	
	private function getItemName($entry)
	{
		//Check for cache
		$cache = $this->cache->get("items/item_name_".$this->realm."_".$entry);
		if ($cache !== false)
		{
			return $cache;
		}
		
		// Use new query in case we support the emulator
		switch ($this->getEmulatorString())
		{
			case 'trinity':
			case 'trinity_cata':
			case 'skyfire':
			case 'arkcore':
			case 'mangos':
				$Statement = "SELECT name FROM item_template WHERE entry=?";
				break;
			case 'arcemu':
				$Statement = "SELECT name FROM items WHERE entry=?";
				break;
			default:
				$info = $this->realms->getRealm($this->realm)->getWorld()->getItem($this->id);
				return $info['name'];
		}
		
		//Get the world database
		$DB = $this->realms->getRealm($this->realm)->getWorld()->getConnection();
		//Run the query
		$query = $DB->query($Statement, array($entry));
		
		if ($DB->_error_message())
		{
			die($DB->_error_message());
		}
		
		if ($query && $query->num_rows() > 0)
		{
			$result = $query->result_array();
			
			// Cache it forever
			$this->cache->save("items/item_name_".$this->realm."_".$entry, $result[0]['name']);
			
			return $result[0]['name'];
		}
		//Check if we have no item data, TrinityCore Cata dont have them items in the DB
		else if ($this->isCataclysm())
		{
			//Pull the item data from the fusion database
			return $this->getItemNameDB2($entry);
		}
		
		return false;
	}

	private function getItemDB2($entry)
	{
		$query = $this->db->query("	SELECT 
										entry, Flags, name, Quality, bonding, InventoryType, MaxDurability, RequiredLevel, ItemLevel, class, subclass, delay, 
										socketColor_1, socketColor_2, socketColor_3, spellid_1, spellid_2, spellid_3, spellid_4, spellid_5, spelltrigger_1, spelltrigger_2, spelltrigger_3, spelltrigger_4, spelltrigger_5, 
										displayid, stat_type1, stat_value1, stat_type2, stat_value2, stat_type3, stat_value3, stat_type4, stat_value4, stat_type5, stat_value5, 
										stat_type6, stat_value6, stat_type7, stat_value7, stat_type8, stat_value8, stat_type9, stat_value9, stat_type10, stat_value10, 
										stackable, socketBonus, itemset, StatScalingFactor, FlagsExtra
		 							FROM `data_cata_".$this->getEmulatorBuild()."_itemtemplate` 
									WHERE entry=? LIMIT 1;", array($entry));
		
		if ($query && $query->num_rows() > 0)
		{
			$result = $query->result_array();
			
			// Cache it forever
			$this->cache->save("items/item_".$this->realm."_".$entry, $result[0]);
			
			return $result[0];
		}
		
		return false;
	}
	
	private function getItemNameDB2($entry)
	{
		$query = $this->db->query("SELECT name FROM `data_cata_".$this->getEmulatorBuild()."_itemtemplate` WHERE entry=? LIMIT 1;", array($entry));
		
		if ($query && $query->num_rows() > 0)
		{
			$result = $query->result_array();
			
			// Cache it forever
			$this->cache->save("items/item_name_".$this->realm."_".$entry, $result[0]['name']);
			
			return $result[0]['name'];
		}
		
		return false;
	}
	
	//This function should return correct values only if statScalingFactor is set
	//Atleast the DPS is correct
	private function getItemDamage($itemLevel, $itemClass, $itemSubClass, $quality, $delay, $statScalingFactor, $inventoryType)
	{
		$minDamage = $maxDamage = $dps = 0.0;
		
		if ((int)$itemClass != 2 || (int)$quality > 6) //If item class is not weapon or the quality is greater than artifact
			return false;
	
		$store = false;
		
		if ((int)$inventoryType > 0xD + 13)
			return false;
		
		// get the right store here
		switch ((int)$inventoryType)
		{
			case 24: //Ammo
				$store = 'itemdamageammo';
				break;
			case 17: //2H Weapon
				if ($this->hasExtraFlag(512)) //ITEM_FLAGS_EXTRA_CASTER_WEAPON
					$store = 'itemdamagetwohandcaster';
				else
					$store = 'itemdamagetwohand';
				break;
			case 15: //Ranged
			case 25: //Thrown
			case 26: //Ranged Right Hand
				switch ((int)$itemSubClass)
				{
					case 19: //Wand
						$store = 'itemdamagewand';
						break;
					case 16: //Thrown
						$store = 'itemdamagethrown';
						break;
					case 2: //Bow
					case 3: //Gun
					case 18: //Crossbow
						$store = 'itemdamageranged';
						break;
					default:
						return false;
				}
				break;
			case 13: //Weapon
			case 21: //Weapon Main Hand
			case 22: //Weapon Off Hand
				if ($this->hasExtraFlag(512)) //ITEM_FLAGS_EXTRA_CASTER_WEAPON
					$store = 'itemdamageonehandcaster';
				else
					$store = 'itemdamageonehand';
				break;
			default:
				return false;
		}
	
		if (!$store)
			return false;

		$damageInfo = $this->getDamageEntry($store, (int)$itemLevel);
		
		if (!$damageInfo)
			return false;
		
		//increase quality by 1 to match the DB Keys
		$quality = (int)$quality + 1;
		
		$dps = (float)$damageInfo['dps_' . (int)$quality];
		
		(float)$avgDamage = $dps * (int)$delay * 0.001;
		
		$minDamage = ((float)$statScalingFactor * -0.5 + 1.0) * $avgDamage;
		$maxDamage = floor((float)($avgDamage * ((float)$statScalingFactor * 0.5 + 1.0) + 0.5));
		
		return array('dps' => $dps, 'damage_min' => floor($minDamage), 'damage_max' => floor($maxDamage));
	}
	
	private function getItemArmor($itemLevel, $itemClass, $itemSubclass, $quality, $inventoryType)
	{
		if ((int)$quality > 6) //If the quality is greater than artifact
			return 0;
	
		// all items but shields
		if ((int)$itemClass != 4 || (int)$itemSubclass != 6)
		{
			//ItemArmorQualityEntry const* armorQuality = sItemArmorQualityStore.LookupEntry($itemlevel);
			$armorQuality = $this->getArmorQualityEntry((int)$itemLevel);
			
			//ItemArmorTotalEntry const* armorTotal = sItemArmorTotalStore.LookupEntry($itemlevel);
			$armorTotal = $this->getArmorTotalEntry((int)$itemLevel);
			
			if (!$armorQuality || !$armorTotal)
				return 0;
			
			//If the type is robe make it chest
			if ((int)$inventoryType == 20) //Robe
				$inventoryType = 5; //Chest
	
			//ArmorLocationEntry const* location = sArmorLocationStore.LookupEntry($inventoryType);
			$location = $this->getArmorLocationEntry((int)$inventoryType);
			
			if (!$location)
				return 0;
			
			//If the subclass is lower than armor cloth
			if ((int)$itemSubclass < 1)
				return 0;
			
			//increase quality by 1 to match the DB Keys
			$quality = (int)$quality + 1;
			
			return floor((float)$armorQuality['value_'.$quality] * (float)$armorTotal['value_'.$itemSubclass] * (float)$location['value_'.$itemSubclass] + 0.5);
		}
	
		// shields
		//ItemArmorShieldEntry const* shield = sItemArmorShieldStore.LookupEntry($itemlevel);
		$armorShield = $this->getArmorShieldEntry((int)$itemlevel);
		
		if (!$armorShield)
			return 0;
		
		//increase quality by 1 to match the DB Keys
		$quality = (int)$quality + 1;
		
		return floor((int)$armorShield['value_'.$quality] + 0.5);
	}
	
	private function getSocketBonus()
	{
		if ($this->additionalData)
		{
			if ($this->additionalData['socketBonus'] > 0)
			{
				$info = $this->getEnchantmentDescription($this->additionalData['socketBonus']);
				
				return ($info ? $info['description'] : false);
			}
			else
				return false;
		}
		
		return false;
	}
	
	private function getItemSet()
	{
		if ($this->additionalData)
		{
			if ($this->additionalData['itemset'] > 0)
			{
				//Check for cache
				$cache = $this->cache->get("items/itemset_".$this->realm."_".$this->additionalData['itemset']);
				
				if ($cache !== false)
				{
					return $cache;
				}
		
				if ($itemset = $this->getItemSetInfo($this->additionalData['itemset']))
				{
					$temp = array(
						'id'			=> $itemset['id'],
						'name' 			=> $itemset['name'],
						'items'			=> false,
						'setBonuses'	=> false
					);
					
					//get the item names for each piece
					for ($i = 1; $i <= 10; $i++)
					{
						$itemid = (int)$itemset['item' . $i];
						//if there is an item
						if ($itemid > 0)
						{
							//get the item name
							$name = $this->getItemName_ItemSet($itemid);
							//The entry from the itemset template is not the actual item entry
							//So we have to find the items that are likly to be part of the set by thier name
							$possibleEntries = $this->getPossibleItemEntries_ItemSet($name);
							
							//merge the entries into string
							if (count($possibleEntries) > 1)
								$possibleEntries = implode(':', $possibleEntries);
							else
								$possibleEntries = (string)$possibleEntries[0];
								
							$temp['items'][] = 	array(
													'entry' 				=> $itemid, 
													'name' 					=> $name,
													'possibleItemEntries'	=> $possibleEntries
												);
						}
					}
					unset($i, $itemid, $name, $possibleEntries);
					
					//get set bonuese
					for ($i = 1; $i <= 8; $i++)
					{
						$spell = (int)$itemset['spell' . $i];
						$requiredItems = (int)$itemset['spell' . $i . '_required_items'];
						
						if ($spell > 0)
						{
							$temp['setBonuses'][] = array('requiredItems' => $requiredItems, 'spell' => $this->getSpellText($spell));
						}
					}
					unset($i, $spell, $requiredItems);
					
					//sort the bonuses by required items
					$this->deep_asort($temp['setBonuses']);
					
					unset($itemset);
					
					// Cache it forever
					$this->cache->save("items/itemset_".$this->realm."_".$this->additionalData['itemset'], $temp);
					
					return $temp;
				}
				else
					return false;
			}
			else
				return false;
		}
		
		return false;
	}
	
	private function deep_asort(&$arr)
	{ 
		asort($arr); 
		foreach ($arr as &$a)
		{ 
			if (is_array($a) && !empty($a))
			{ 
				$this->deep_asort($a); 
			} 
		} 
	} 
	
	/**
	 * Get the sockets
	 * @param Array $item
	 * @return String
	 */
	private function getSockets($item)
	{
		$output = "";

		$meta = "<span class='socket-slot socket-required socket-meta q0' data-socket-color='1'>Meta Socket</span><br />";
		$red = "<span class='socket-slot socket-required socket-red q0' data-socket-color='2'>Red Socket</span><br />";
		$yellow = "<span class='socket-slot socket-required socket-yellow q0' data-socket-color='4'>Yellow Socket</span><br />";
		$blue = "<span class='socket-slot socket-required socket-blue q0' data-socket-color='8'>Blue Socket</span><br />";

		for($i = 1; $i <= 3; $i++)
		{
			switch($item['socketColor_'.$i])
			{
				case 1: $output .= $meta; break;
				case 2: $output .= $red; break;
				case 4: $output .= $yellow; break;
				case 8: $output .= $blue; break;
			}
		}

		return $output;
	}

	/**
	 * Calculate DPS
	 * @param Float $min
	 * @param Float $max
	 * @param Float $speed
	 * @return Float
	 */
	private function getDPS($min, $max, $speed)
	{
		if($speed > 0)
		{
			return round((($min + $max) / 2) / $speed, 1);
		}
	}

	/**
	 * Get the attributes
	 * @param Array $item
	 * @return Array
	 */
	private function getAttributes($item)
	{
		$types = $this->config->item("stats");

		$statCount = 10;
		$attributes = array(
				"spells" => array(),
				"regular" => array()
			);

		for($i = 1; $i <= $statCount; $i++)
		{
			if(!empty($item['stat_value'.$i]) && array_key_exists($item['stat_type'.$i], $types))
			{
				$type = "spells";
			
				// Mana/health
				if(in_array($item['stat_type'.$i], array(42,46)))
				{
					$stat = "<span class='q2'>Equip: Restores ".$item['stat_value'.$i]." ".$types[$item['stat_type'.$i]]."</span><br />";
				}
				elseif($item['stat_type'.$i] > 7 && !in_array($item['stat_type'.$i], array(42,46)))
				{
					$stat = "<span class='q2'>Equip: Increases your ".$types[$item['stat_type'.$i]]."by ".$item['stat_value'.$i].".</span><br />";
				}
				else
				{
					if(array_key_exists($item['stat_type'.$i], $types))
					{
						$type = "regular";
						$stat = "+" . $item['stat_value'.$i] . " " . $types[$item['stat_type'.$i]]."<br />";
					}
				}

				array_push($attributes[$type], array('id' => $item['stat_value'.$i], 'text' => $stat));
			}
		}

		return $attributes;
	}

	/**
	 * Get attribute by the ID if any
	 * @param Int $id
	 * @return Int
	 */
	private function getAttributeById($id, $item)
	{
		$statCount = 10;
		
		for($i = 1; $i <= $statCount; $i++)
		{
			if($item['stat_type'.$i] == $id)
			{
				return $item['stat_value'.$i];
			}
		}

		return false;
	}

	/**
	 * Get the spells
	 * @param Array $item
	 * @return Array
	 */
	private function getSpells($item)
	{
		$spelltriggers = $this->config->item("spelltriggers");

		$spellCount = 5;
		$spells = array();

		for($i = 1; $i <= $spellCount; $i++)
		{
			if(!empty($item['spellid_'.$i]))
			{
				$data = array(
						"id" => $item['spellid_'.$i],
						"trigger" => $spelltriggers[$item['spelltrigger_'.$i]],
						"text" => $this->getSpellText($item['spellid_'.$i])
					);

				array_push($spells, $data);
			}
		}
		
		return $spells;
	}

	private function getSpellText($id)
	{
		//Check for cache
		$cache = $this->cache->get("spells/spell_data_".$this->realm."_setbonus_".(int)$id);
		
		if($cache !== false)
		{
			$data = json_decode($cache['data'], true);
			return $data['tooltip'];
		}
		
		//Pull data about this pell
		$json = @file_get_contents(base_url()."spell_data/index/".$this->realm."/".(int)$id."/setbonus");
		
		if ($json)
		{
			$data = json_decode($json, true);
			
			return $data['tooltip'];
		}
		
		return false;
	}

	/* Old Function
	private function getSpellText($id)
	{
		$cache = $this->cache->get("spells/spell_".$id);

		if($cache !== false)
		{
			return $cache;
		}
		else
		{
			$query = $this->db->query("SELECT spellText FROM spelltext_en WHERE spellId=? LIMIT 1", array($id));

			// Check for results
			if($query->num_rows() > 0)
			{
				$row = $query->result_array();

				$data = $row[0]['spellText'];
			}
			else
			{
				$data = false;
			}
			
			$this->cache->save("spells/spell_".$id, $data);

			return $data;
		}
	}
	*/
	
	/**
	 * Get the type
	 * @param Int $class
	 * @param Int $subclass
	 * @return String
	 */
	private function getType($class, $subclass)
	{
		// Weapons
		if($class == 2)
		{
			$sub = $this->config->item("weapon_sub");

			return $sub[$subclass];
		}

		// Armor
		elseif($class == 4)
		{
			$sub = $this->config->item("armor_sub");

			return $sub[$subclass];
		}

		// Anything else
		else
		{
			return null;
		}
	}

	/**
	 * Get flags as an array
	 * @param Int $flags
	 * @return Array
	 */
	private function getFlags($flags)
	{
		$bits = array();

		for($i = 1; $i <= $flags; $i *= 2)
		{
			if(($i & $flags) > 0)
			{
				array_push($bits, $i);
			}
		}

		return $bits;
	}

	/**
	 * Check if our flags array contains the flag
	 * @param Int $flag
	 * @return Boolean
	 */
	private function hasFlag($flag)
	{
		if(in_array($flag, $this->flags))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	/**
	 * Check if our extraFlags array contains the flag
	 * @param Int $flag
	 * @return Boolean
	 */
	private function hasExtraFlag($flag)
	{
		if(in_array($flag, $this->extraFlags))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	######################################################
	################# SQL DATA FUNCTIONS #################
	
	private function getDamageEntry($store, $itemLevel)
	{
		$query = $this->db->query("SELECT * FROM `data_cata_".$this->getEmulatorBuild()."_".$store."` WHERE `id` = ? LIMIT 1;", array($itemLevel));
		
		if ($this->db->_error_message())
		{
			die($this->db->_error_message());
		}
		
		if ($query && $query->num_rows() > 0)
		{
			$result = $query->result_array();
			return $result[0];
		}
		
		unset($query);
		
		return false;
	}
	
	private function getArmorQualityEntry($itemLevel)
	{
		$query = $this->db->query("SELECT * FROM `data_cata_".$this->getEmulatorBuild()."_itemarmorquality` WHERE `id` = ? LIMIT 1;", array($itemLevel));
		
		if ($query && $query->num_rows() > 0)
		{
			$result = $query->result_array();
			return $result[0];
		}
		
		unset($query);
		
		return false;
	}
	
	private function getArmorTotalEntry($itemLevel)
	{
		$query = $this->db->query("SELECT * FROM `data_cata_".$this->getEmulatorBuild()."_itemarmortotal` WHERE `id` = ? LIMIT 1;", array($itemLevel));
		
		if ($query && $query->num_rows() > 0)
		{
			$result = $query->result_array();
			return $result[0];
		}
		
		unset($query);
		
		return false;
	}
	
	private function getArmorLocationEntry($inventoryType)
	{
		$query = $this->db->query("SELECT * FROM `data_cata_".$this->getEmulatorBuild()."_armorlocation` WHERE `InventoryType` = ? LIMIT 1;", array($inventoryType));
		
		if ($query && $query->num_rows() > 0)
		{
			$result = $query->result_array();
			return $result[0];
		}
		
		unset($query);
		
		return false;
	}
	
	private function getArmorShieldEntry($itemLevel)
	{
		$query = $this->db->query("SELECT * FROM `data_cata_".$this->getEmulatorBuild()."_itemarmorshield` WHERE `id` = ? LIMIT 1;", array($itemLevel));
		
		if ($query && $query->num_rows() > 0)
		{
			$result = $query->result_array();
			return $result[0];
		}
		
		unset($query);
		
		return false;
	}
	
	private function getItemName_ItemSet($item)
	{
		switch ($this->getEmulatorString())
		{
			case 'trinity_cata':
				return $this->getItemName($item);
			case 'trinity':
			case 'mangos':
			case 'arcemu':
				return $this->getItemName_ItemSet_Local($item);
		}
		
		$DB = $this->realms->getRealm($this->realm)->getWorld()->getConnection();
		
		$statements['arkcore']		= 
		$statements['skyfire']		= "SELECT `name` FROM `item_set_names` WHERE `entry` = ? LIMIT 1;";
		
		$query = $DB->query($statements[$this->getEmulatorString()], array($item));
		
		if ($query && $query->num_rows() > 0)
		{
			$result = $query->result_array();
			
			return $result[0]['name'];
		}
		
		unset($query, $statements, $DB);
		
		return false;
	}
	
	private function getItemName_ItemSet_Local($item)
	{
		$query = $this->db->query("SELECT `name` FROM `data_wotlk_itemsetnames` WHERE `entry` = ? LIMIT 1;", array($item));
		
		if ($query && $query->num_rows() > 0)
		{
			$result = $query->result_array();
			
			return $result[0]['name'];
		}
		
		unset($query);
		
		return false;
	}
	
	private function getPossibleItemEntries_ItemSet($name)
	{
		$DB = $this->realms->getRealm($this->realm)->getWorld()->getConnection();
		
		switch ($this->getEmulatorString())
		{
			case 'trinity':
			case 'trinity_cata':
			case 'arkcore':
			case 'skyfire':
			case 'mangos':
				$statement = "SELECT `entry` FROM `item_template` WHERE `name` LIKE CONCAT('%', ?, '%');";
				break;
			case 'arcemu':
				$statement = "SELECT `entry` FROM `items` WHERE `name1` LIKE CONCAT('%', ?, '%');";
				break;
		}
		
		$query = $DB->query($statement, array($name));
		
		if ($query && $query->num_rows() > 0)
		{
			$result = $query->result_array();
		}
		else if ($this->isCataclysm())
		{
			$query = $this->db->query("SELECT `entry` FROM `data_cata_".$this->getEmulatorBuild()."_itemtemplate` WHERE `name` LIKE CONCAT('%', ?, '%');", array($name));
			
			if ($query && $query->num_rows() > 0)
			{
				$result = $query->result_array();
			}
		}
		
		unset($query, $statements, $DB);
		
		// If we have the db results
		if (isset($result))
		{
			//temp array
			$temp = array();
			//merge into single dementional array
			foreach ($result as $key => $item)
			{
				$temp[] = $item['entry'];
			}
			unset($result, $key, $item);
			
			return $temp;
		}
		
		return false;
	}
	
	private function getItemSetInfo($id)
	{
		if ($this->isCataclysm())
			$prefix = 'data_cata_' . $this->getEmulatorBuild();
		else
			$prefix = 'data_wotlk';
			
		$query = $this->db->query("SELECT * FROM `".$prefix."_itemset` WHERE `id` = ? LIMIT 1;", array($id));
		
		if ($query && $query->num_rows() > 0)
		{
			$result = $query->result_array();
			return $result[0];
		}
		
		unset($query);
		
		return false;
	}
	
	private function getEnchantmentDescription($id)
	{
		if ($this->isCataclysm())
			$prefix = 'data_cata_' . $this->getEmulatorBuild();
		else
			$prefix = 'data_wotlk';
		
		$query = $this->db->query("	SELECT `id`, `description`, `GemID` 
									FROM `".$prefix."_spellitemenchantment`   
									WHERE `id` = ? 
									LIMIT 1;", array($id));
		
		if ($query && $query->num_rows() > 0)
		{
			$result = $query->result_array();
			return $result[0];
		}
		
		unset($query);
		
		return false;
	}
}