<?php

class Activation_model extends CI_Model
{
	public function add($username, $password, $email, $expansion, $recruiter)
	{
		$data = array(
			'username' => $username,
			'email' => $email,
			'password' =>  $this->user->createHash($username, $password),
			'expansion' => $expansion,
			'timestamp' => time(),
			'ip' => $_SERVER['REMOTE_ADDR'],
			'key' => sha1($username.$email.$password.time()),
			'recruiter' => $recruiter
		);

		$this->db->insert("pending_accounts", $data);

		return $data['key'];
	}

	public function getAccount($key)
	{
		$query = $this->db->query("SELECT * FROM pending_accounts WHERE `key`=?", array($key));

		if($query->num_rows())
		{
			$row = $query->result_array();

			return $row[0];
		}
		else
		{
			return false;
		}
	}

	public function remove($id, $username, $email)
	{
		$this->db->query("DELETE FROM pending_accounts WHERE id=? OR username=? OR email=?", array($id, $username, $email));
	}
}