<?php
class Recruitafriend extends MX_Controller
{
	
	function __construct()
	{
		//Call the constructor of MX_Controller
		parent::__construct();
		
		//Make sure that we are logged in
		$this->user->is_logged_in();

		$this->load->config('recruitafriend');
	}
	
	public function index()
	{
		$this->template->setTitle("Referrals");
		$auth = $this->external_account_model->getConnection();
		$query = $auth->query("SELECT ".column("account", "id")." as id, ".column("account", "username")." as username FROM ".table("account")." WHERE recruiter = ".$this->user->getId());
		$row = $query->num_rows() > 0 ? $query->result_array() : false;
		$tab = array();
		if(is_array($row))
		{
			foreach($row as $value)
			{
				$tab[] = array(
					"id" => $value['id'],
					"username" => $value['username'],
					"canClaim" => $this->canClaim($value['id'])
					);
			}
		}
		else
		{
			$tab = false;
		}
		$data = array(
			"accounts" => $tab,
			"ref_url" => $this->template->page_url."register?r=".$this->user->getId(),
			"base_url" => $this->template->page_url
		);
		$this->template->view($this->template->loadPage("page.tpl", array(
			"module" => "default", 
			"headline" => "<span style='cursor:pointer;' onClick='window.location=\"".$this->template->page_url."ucp\"'>UCP</span> &rarr; Referrals",
			"content" => $this->template->loadPage("recruit.tpl", $data)
		)));
	}
	private function canClaim($id)
	{
	/*
	Returns:
	0 : user doesn't exist
	1 : available
	2 : already claimed
	3 : time < required
	4 : not referal (shouldn't happen)
	*/
		if(!$this->external_account_model->userExists($id))
			return 0;
			
		$auth = $this->external_account_model->getConnection();
		$query = $auth->query("SELECT * FROM ".table("account")." WHERE ".column("account", "id")." = ".$id." AND recruiter = ".$this->user->getId());
		if($query->num_rows() > 0)
		{
			$query = $this->db->query("SELECT * FROM recruit_a_friend_claimed WHERE recruiter = ".$this->user->getId()." AND referral = ".$id);
			if($query->num_rows() > 0)
			{
				return 2;
			}
			else
			{
				if($this->config->item('recruit_realm_id') == 0)
				{
					$totaltime = 0;
					$realms = $this->realms->getRealms();
					foreach($realms as $realm)
					{
						$characterDb = $realm->getCharacters()->getConnection();
						$id = $characterDb->escape($id);
						$query = $characterDb->query("SELECT sum(totaltime) as time FROM ".table("characters")." WHERE ".column("characters", "account")." = ".$id);
						$row = $query->row();
						if($row->time)
							$totaltime += $row->time;
					}
					if($totaltime >= $this->config->item('recruit_min_time'))
					{
						return 1;
					}
					else
					{
						return 3;
					}
				}
				else
				{
					$realm = $this->realms->getRealm($this->config->item('recruit_realm_id'));
					$characterDb = $realm->getCharacters()->getConnection();
					$id = $characterDb->escape($id);
					$query = $characterDb->query("SELECT sum(totaltime) as time FROM ".table("characters")." WHERE ".column("characters", "account")." = ".$id);
					$row = $query->row();
					if($row->time && $row->time >= $this->config->item('recruit_min_time'))
					{
						return 1;
					}
					else
					{
						return 3;
					}
				}
			}
		}
		else
			return 4;
	}
	public function claim($id)
	{
		switch($this->canClaim($id))
		{
			case 0:
				$this->template->view($this->template->loadPage("page.tpl", array(
				"module" => "default", 
				"headline" => "<span style='cursor:pointer;' onClick='window.location=\"".$this->template->page_url."ucp\"'>UCP</span> &rarr; Referrals",
				"content" => "Error : The account doesn't exist anymore!"
				)));
				break;
			case 1:
				$this->db->query("INSERT INTO recruit_a_friend_claimed (recruiter, referral) VALUES (".$this->user->getId().", ".$id.")");
					if($this->config->item('recruit_reward_type') == "vp")
						$this->user->setVp($this->user->getVp() + $this->config->item('recruit_reward_amount'));
					else
						$this->user->setDp($this->user->getDp() + $this->config->item('recruit_reward_amount'));
					$this->template->view($this->template->loadPage("page.tpl", array(
					"module" => "default", 
					"headline" => "<span style='cursor:pointer;' onClick='window.location=\"".$this->template->page_url."ucp\"'>UCP</span> &rarr; Referrals",
					"content" => "You have been awarded ".$this->config->item('recruit_reward_amount')." ".strtoupper($this->config->item('recruit_reward_type'))." for your referral
					<script>
					setTimeout(function(){
					window.location = '".$this->template->page_url."recruitafriend/';
					}, 2000);
					</script>"
					)));
					break;
			case 2:
				$this->template->view($this->template->loadPage("page.tpl", array(
				"module" => "default", 
				"headline" => "<span style='cursor:pointer;' onClick='window.location=\"".$this->template->page_url."ucp\"'>UCP</span> &rarr; Referrals",
				"content" => "Error : You already claimed your reward for this account")));
				break;
			case 3:
				$this->template->view($this->template->loadPage("page.tpl", array(
					"module" => "default", 
					"headline" => "<span style='cursor:pointer;' onClick='window.location=\"".$this->template->page_url."ucp\"'>UCP</span> &rarr; Referrals",
					"content" => "Error : You can not claim your reward for this account yet!")));
				break;
			default:
				$this->template->view($this->template->loadPage("page.tpl", array(
					"module" => "default", 
					"headline" => "<span style='cursor:pointer;' onClick='window.location=\"".$this->template->page_url."ucp\"'>UCP</span> &rarr; Referrals",
					"content" => "Error : Something went wrong!")));
					break;
		}
	}
}