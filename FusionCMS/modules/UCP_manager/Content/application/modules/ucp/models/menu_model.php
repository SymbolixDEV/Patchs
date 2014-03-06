<?php

class Menu_model extends CI_Model
{
	public function getMenuLinks()
	{
		$query = $this->db->query("SELECT * FROM menu_ucp ORDER BY `order` ASC");
		
		if($query->num_rows() > 0)
		{
			return $query->result_array();
		}
		else 
		{
			return false;
		}
	}
	
	public function getMenuLink($id)
	{
		$query = $this->db->query("SELECT * FROM menu_ucp WHERE id=?", array($id));
		
		if($query->num_rows() > 0)
		{
			$result = $query->result_array();
			return $result[0];
		}
		else 
		{
			return false;
		}
	}
	
	public function delete($id)
	{
		if($this->db->query("DELETE FROM menu_ucp WHERE id = ?", array($id)))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	public function edit($id, $data)
	{
		$this->db->where('id', $id);
		$this->db->update('menu_ucp', $data);
	}
	
	public function add($name, $link, $image, $direct_link, $permission, $module)
	{
		$data = array(
			"name" => $name,
			"link" => $link,
			"image" => $image,
			"direct_link" => $direct_link,
			"permission" => $permission,
			"permissionModule" => $module
		);

		$this->db->insert("menu_ucp", $data);
		
		$query = $this->db->query("SELECT id FROM menu_ucp ORDER BY id DESC LIMIT 1");
		$row = $query->result_array();

		$this->db->query("UPDATE menu_ucp SET `order`=? WHERE id=?", array($row[0]['id'], $row[0]['id']));

		return $row[0]['id'];
	}

	public function getOrder($id)
	{
		$query = $this->db->query("SELECT `order` FROM menu_ucp WHERE `id`=? LIMIT 1", array($id));

		if($query->num_rows() > 0)
		{
			$row = $query->result_array();

			return $row[0]['order'];
		}
		else
		{
			return false;
		}
	}

	public function getPreviousOrder($order)
	{
		$query = $this->db->query("SELECT `order`, id FROM menu_ucp WHERE `order` < ? ORDER BY `order` DESC LIMIT 1", array($order));

		if($query->num_rows() > 0)
		{
			$row = $query->result_array();

			return $row[0];
		}
		else
		{
			return false;
		}
	}

	public function getNextOrder($order)
	{
		$query = $this->db->query("SELECT `order`, id FROM menu_ucp WHERE `order` > ? ORDER BY `order` ASC LIMIT 1", array($order));

		if($query->num_rows() > 0)
		{
			$row = $query->result_array();

			return $row[0];
		}
		else
		{
			return false;
		}
	}

	public function setOrder($id, $order)
	{
		$this->db->query("UPDATE menu_ucp SET `order`=? WHERE `id`=? LIMIT 1", array($order, $id));
	}
}