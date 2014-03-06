<?php

class Icon extends MX_Controller
{
	private $EmulatorSimpleString = '';
	
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
	 * Get an item's icon display name and cache it
	 * @param Int $realm
	 * @param Int $id
	 * @param Int $isDisplayId
	 * @return String
	 */
	public function get($realm = false, $id = false, $isDisplayId = 0)
	{		
		// Check if ID and realm is valid
		if($id != false && is_numeric($id) && $realm != false)
		{
			// Get the emulator string
			$this->EmulatorSimpleString = str_replace(array('_ra', '_soap', '_rbac'), '', $this->realms->getRealm($realm)->getConfig('emulator'));
			
			// It is already a display ID
			if($isDisplayId == 1)
			{
				$icon = $this->getDisplayName($id);
			}
			else
			{
				$displayId = $this->getDisplayId($id, $realm);
				
				if($displayId != false)
				{
					$icon = $this->getDisplayName($displayId);

					if(substr($icon, 0, 3) == pack("CCC", 0xef, 0xbb, 0xbf))
					{
						$icon = substr($icon, 3);
					}
				}
				else
				{
					$icon = "inv_misc_questionmark";
				}
			}
			
			die($icon);
		}
	}

	/**
	 * Get the display ID of an item
	 * @param Int $item
	 * @return Int
	 */
	private function getDisplayId($entry, $realm)
	{
		$cache = $this->cache->get("items/item_displayid_".$realm."_".$entry);
		if ($cache)
		{
			return $cache;
		}
		
		$realmObj = $this->realms->getRealm($realm);
		$item = $realmObj->getWorld()->getItem($entry);
		
		if ((!$item || $item == "empty") && $this->getEmulatorString() == 'trinity_cata')
		{
			return $this->getDisplayIdDB2($entry, $realm);
		}
		
		//Cache the display id
		$this->cache->save("items/item_displayid_".$realm."_".$entry, $item['displayid']);
		
		return $item['displayid'];
	}
	
	private function getDisplayIdDB2($entry, $realm)
	{
		$query = $this->db->query("SELECT displayid FROM `data_cata_".$this->getEmulatorBuild()."_itemtemplate` WHERE entry=? LIMIT 1;", array($entry));
		
		if ($this->db->_error_message())
		{
			die($this->db->_error_message());
		}
		
		if ($query && $query->num_rows() > 0)
		{
			$result = $query->result_array();
			
			//Cache the display id
			$this->cache->save("items/item_displayid_".$realm."_".$entry, $result[0]['displayid']);
			
			return $result[0]['displayid'];
		}
		
		return false;
	}
	
	/**
	 * Get the display name from the raxezdev display ID API
	 * @param Int $displayId
	 * @return String
	 */
	private function getDisplayName($displayId)
	{
		$cache = $this->cache->get("items/display_".$displayId);
		
		// Can we use the cache?
		if($cache !== false)
		{
			$name = $cache;
		}
		else
		{
			$icon = $this->getIcon($displayId);
			
			if(!$icon)
			{
				$name = "inv_misc_questionmark";
			}
			else
			{
				$name = str_replace(' ', '-', strtolower($icon));

				// In case it wasn't found: show ?-icon
				if(empty($name))
				{
					$name = "inv_misc_questionmark";
				}
			}

			// Make sure to cache
			$this->cache->save("items/display_".$displayId, $name);
		}
		
		return $name;
	}
	
	private function getIcon($DisplayId)
	{
		if ($this->isCataclysm())
			$prefix = 'data_cata_' . $this->getEmulatorBuild();
		else
			$prefix = 'data_wotlk';
			
		$query = $this->db->query("SELECT * FROM `".$prefix."_itemdisplayinfo` WHERE `id` = ? LIMIT 1;", array($DisplayId));
		
		if ($this->db->_error_message())
		{
			die($this->db->_error_message());
		}
		
		if ($query && $query->num_rows() > 0)
		{
			$result = $query->result_array();
			
			return $result[0]['iconname'];
		}
		
		unset($query);
		
		return false;
	}
	
	/*
	private function findRetailItem($id)
	{
		// Get the item ID
		$query = $this->db->query("SELECT entry FROM item_display WHERE displayid=? LIMIT 1", array($id));

		// Check for results
		if($query->num_rows() > 0)
		{
			$row = $query->result_array();

			return $row[0]['entry'];
		}
		else
		{
			return false;
		}
	}
	
	private function getIconName($item)
	{
		// Get the item XML data
		$xml = file_get_contents("http://www.wowhead.com/item=".$item."&xml");

		// In case it wasn't found: show ?-icon
		if(empty($xml))
		{
			$icon = "inv_misc_questionmark";
		}
		else
		{
			// Convert the data to an array
			$itemData = $this->xmlToArray($xml);

			// Make sure the icon key is set
			$icon = (isset($itemData['item']['icon'])) ? strtolower($itemData['item']['icon']) : "inv_misc_questionmark";
		}

		return $icon;
	}
	*/
	
	/**
	 * Convert XML data to an array
	 * @param String $xml
	 * @return Array
	 */
	private function xmlToArray($xml)
	{
		$xml = simplexml_load_string($xml);
		$json = json_encode($xml);
		$array = json_decode($json, TRUE);

		return $array;
	}
}