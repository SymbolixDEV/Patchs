<?php

class Admin extends MX_Controller
{
	const MODULE_NAME			= 'ucp';
	const MODULE_PATH 			= 'modules/ucp';
	
	public function __construct()
	{
		// Make sure to load the administrator library!
		$this->load->library('administrator');
		$this->load->model("menu_model");
		
		parent::__construct();

		requirePermission("manageAdmin");
	}

	public function index($page = 0)
	{
		// Change the title
		$this->administrator->setTitle("UCP Management");
	
		$links = $this->menu_model->getMenuLinks();

		if($links)
		{
			foreach($links as $key => $value)
			{
				// Shorten the link if necessary
				if(strlen($value['link']) > 12)
				{
					$links[$key]['link_short'] = mb_substr($value['link'], 0, 12) . '...';
				}
				else
				{
					$links[$key]['link_short'] = $value['link'];
				}

				// Add the website path if internal link
				if(!preg_match("/https?:\/\//", $value['link']))
				{
					$links[$key]['link'] = $this->template->page_url . $value['link'];
				}

				$links[$key]['name'] = langColumn($links[$key]['name']);
			}
		}
		
		// Get all the roles/permissions and format em
		$Permissions = $this->getAllPermissions();
		$PermissionOptions = array();
		
		foreach ($Permissions as $module => $moduleData)
		{
			if ($moduleData['permissions'] && count($moduleData['permissions']) > 0)
			{
				// Start the option group
				$PermissionOptions[] = '<optgroup label="' . $moduleData['name'] . '">';
				
				// Set the children
				foreach ($moduleData['permissions'] as $permission => $permissionData)
				{
					$PermissionOptions[] = '<option value="'.$module.':'.$permission.'">' . $permission . ' ('.$permissionData['description'].')</option>';
				}
				
				// Close the option group
				$PermissionOptions[] = '</optgroup>';
			}
		}
		
		// Prepare my data
		$data = array(
			'url' => $this->template->page_url,
			'links' => $links,
			'PermissionOptions' => $PermissionOptions,
			'images' => $this->GetGalleryImages()
		);

		// Load my view
		$output = $this->template->loadPage("admin.tpl", $data);

		// Put my view in the main box with a headline
		$content = $this->administrator->box('User Control Panel Management', $output);

		// Output my content. The method accepts the same arguments as template->view
		$this->administrator->view($content, self::MODULE_PATH . "/css/admin.css", self::MODULE_PATH . "/js/admin.js");
	}

	private function GetGalleryImages()
	{
		//path to directory to scan
		$directory = "application/" . self::MODULE_PATH . "/images/gallery/";
		
		//get all images
		$files = glob($directory . "*.{jpg,gif,png,JPG,GIF,PNG}", GLOB_BRACE|GLOB_MARK);
		
		foreach ($files as $k => $file)
		{
			$files[$k] = pathinfo($file);
			$files[$k] = $files[$k]['basename'];
		}
		
		return $files;
	}
	
	private function getAllPermissions()
	{
		$modules = array();

		foreach(glob("application/modules/*") as $module)
		{
			if(is_dir($module))
			{
				$data = file_get_contents($module."/manifest.json");
				$manifest = json_decode($data, true);

				$module = preg_replace("/^application\/modules\//", "", $module);

				if(is_array($manifest))
				{
					$modules[$module]['name'] = (array_key_exists("name", $manifest)) ? $manifest['name'] : $module;
					$modules[$module]['permissions'] = (array_key_exists("permissions", $manifest)) ? $manifest['permissions'] : false;
				}
			}
		}

		return $modules;
	}
	
	public function create()
	{
		$name = $this->input->post('name');
		$link = $this->input->post('link');
		$image = $this->input->post('image');
		$permission = $this->input->post('permission');
		$direct_link = $this->input->post('direct_link');
		
		if (!$image || $image == '')
		{
			if ($image = $this->GetGalleryImages())
				$image = $image[0];
		}
		
		if ($permission && $permission != '0')
		{
			list($module, $permission) = explode(':', $permission);
		}
		else
		{
			$permission = $module = NULL;
		}
		
		$id = $this->menu_model->add($name, $link, $image, $direct_link, $permission, $module);
		
		die('window.location.reload(true)');
	}
	
	public function delete($id)
	{
		if($this->menu_model->delete($id))
		{
			die("success");
		}
		else
		{
			die("An error occurred while trying to delete this menu link.");
		}
		
	}

	public function edit($id = false)
	{
		if(!is_numeric($id) || !$id)
		{
			die();
		}

		$link = $this->menu_model->getMenuLink($id);
	
		if(!$link)
		{
			show_error("There is no link with ID ".$id);

			die();
		}

		// Change the title
		$this->administrator->setTitle(langColumn($link['name']));
		
		// Get all the roles/permissions and format em
		$Permissions = $this->getAllPermissions();
		$PermissionOptions = array();
		
		foreach ($Permissions as $module => $moduleData)
		{
			if ($moduleData['permissions'] && count($moduleData['permissions']) > 0)
			{
				// Start the option group
				$PermissionOptions[] = '<optgroup label="' . $moduleData['name'] . '">';
				
				// Set the children
				foreach ($moduleData['permissions'] as $permission => $permissionData)
				{
					$PermissionOptions[] = '<option value="'.$module.':'.$permission.'" ' . ($module == $link['permissionModule'] && $permission == $link['permission'] ? 'selected="selected"' : '') . '>' . $permission . ' ('.$permissionData['description'].')</option>';
				}
				
				// Close the option group
				$PermissionOptions[] = '</optgroup>';
			}
		}
		
		// Prepare my data
		$data = array(
			'url' => $this->template->page_url,
			'link' => $link,
			'PermissionOptions' => $PermissionOptions,
			'images' => $this->GetGalleryImages()
		);

		// Load my view
		$output = $this->template->loadPage("admin_edit.tpl", $data);

		// Put my view in the main box with a headline
		$content = $this->administrator->box('<a href="'.$this->template->page_url.'ucp/admin">User Control Panel Management</a> &rarr; '.langColumn($link['name']), $output);

		// Output my content. The method accepts the same arguments as template->view
		$this->administrator->view($content, self::MODULE_PATH . "/css/admin.css", self::MODULE_PATH . "/js/admin.js");
	}

	public function move($id = false, $direction = false)
	{
		if(!$id || !$direction)
		{
			die();
		}
		else
		{
			$order = $this->menu_model->getOrder($id);

			if(!$order)
			{
				die();
			}
			else
			{
				if($direction == "up")
				{
					$target = $this->menu_model->getPreviousOrder($order);
				}
				else
				{
					$target = $this->menu_model->getNextOrder($order);
				}

				if(!$target)
				{
					die();
				}
				else
				{
					$this->menu_model->setOrder($id, $target['order']);
					$this->menu_model->setOrder($target['id'], $order);
				}
			}
		}
	}

	public function save($id = false)
	{
		if(!$id || !is_numeric($id))
		{
			die();
		}
		
		$image = $this->input->post('image');
		
		if (!$image || $image == '')
		{
			if ($image = $this->GetGalleryImages())
				$image = $image[0];
		}
		
		$permission = $this->input->post('permission');
		
		if ($permission && $permission != '0')
		{
			list($module, $permission) = explode(':', $permission);
		}
		else
		{
			$permission = $module = NULL;
		}
		
		$data['name'] = $this->input->post('name');
		$data['link'] = $this->input->post('link');
		$data['image'] = $image;
		$data['permission'] = $permission;
		$data['permissionModule'] = $module;
		$data['direct_link'] = $this->input->post('direct_link');
		
		$this->menu_model->edit($id, $data);

		die('window.location="'.$this->template->page_url.'ucp/admin"');
	}
}