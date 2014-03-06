<?php

class Spell_data extends MX_Controller
{
	private $realm;
	private $EmulatorSimpleString = '';
	
	function __construct()
	{
		parent::__construct();
		
		$this->load->model("wotlk_model");
		$this->load->model("cata_model");
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
	
	private function getDataModel()
	{
		if ($this->isCataclysm())
		{
			$this->cata_model->setBuild($this->getEmulatorBuild());
			return $this->cata_model;
		}
		
		//default
		return $this->wotlk_model;
	}
	
	/**
	 * Initialize
	 */
	public function index($realm = false, $spell = false, $type = 'talent', $debug = false)
	{
		$cache = $this->cache->get("spells/spell_data_".($realm ? $realm : 0)."_".$type."_".$spell);
		
		if($cache !== false)
		{
			echo $cache['data'];
			exit;
		}
		else
		{
			$this->realm = $this->realms->getRealm($realm);
			$this->EmulatorSimpleString = str_replace(array('_ra', '_soap', '_rbac'), '', $this->realm->getConfig('emulator'));
			
			//Set the realmid for the data model
			$this->getDataModel()->setRealm($realm);
			//Check for debug mode
			if ($debug !== false)
				$this->getDataModel()->enableDebugMode();
				
			//Set them json headers
			header('Cache-Control: no-cache, must-revalidate');
			header('Expires: Mon, 26 Jul 1997 05:00:00 GMT');
			if ($debug === false)
				header('Content-type: application/json');
			
			//switch between the spell data type
			switch ($type)
			{
				case 'talent':
					$info = $this->handleTalent($spell);
					break;
				case 'glyph':
					$info = $this->handleGlyph($spell);
					break;
				case 'setbonus':
					$info = $this->handleSetBonus($spell);
					break;
			}
			
			if ($info['cacheIt'])
				$this->cache->save("spells/spell_data_".($realm ? $realm : 0)."_".$type."_".$spell, array('data' => $info['data']), strtotime('1 month', 0));
				
			echo $info['data'];
		}
		exit;
	}
	
	private function handleTalent($spell)
	{
		$cacheIt = false;
		
		if($this->getDataModel()->Exists($spell))
		{
			$info['tooltip'] = $this->getDataModel()->getSpellTooltip($spell);
			
			$data = json_encode($info);
			$cacheIt = true;
		}
		else
		{
			$data = json_encode(array('tooltip' => 'The website was unable to retrive some data.'));
		}
		
		return array('data' => $data, 'cacheIt' => $cacheIt);
	}
	
	private function handleSetBonus($spell)
	{
		$cacheIt = false;
		
		if($this->getDataModel()->Exists($spell))
		{
			$info['tooltip'] = $this->getDataModel()->getSpellDesc($spell);
			
			$data = json_encode($info);
			$cacheIt = true;
		}
		else
		{
			$data = json_encode(array('tooltip' => 'The website was unable to retrive some data.'));
		}
		
		return array('data' => $data, 'cacheIt' => $cacheIt);
	}
	
	private function handleGlyph($glyph)
	{
		$cacheIt = false;
		
		if($this->getDataModel()->glyphExists($glyph))
		{
			if ($info = $this->getDataModel()->getGlyphInfo($glyph))
			{
				$info['tooltip'] = $this->getDataModel()->getSpellDesc($info['spellid']);
				
				$data = json_encode($info);
				$cacheIt = true;
			}
		}
		else
		{
			$data = json_encode(array('name' => 'Unknown', 'rank' => 'Rank 0', 'tooltip' => 'The website was unable to retrive some data.'));
		}
		
		return array('data' => $data, 'cacheIt' => $cacheIt);
	}
}