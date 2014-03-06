<?php

class Register extends MX_Controller
{
	private $usernameError;
	private $emailError;

	public function __construct()
	{
		//Call the constructor of MX_Controller
		parent::__construct();
		
		//make sure that we are not yet logged in
		Modules::run('login/is_not_logged_in');
		
		//Load url and form library
		$this->load->helper(array('form', 'url'));
		$this->load->library('form_validation');

		$this->load->config('bridge');
		$this->load->config('activation');
		$this->load->config('captcha');

		$this->load->model('activation_model');
	}
	
	public function index()
	{
		$this->template->setTitle("Register");

		//Load the form validations for if they tried to sneaky bypass our js system
		$this->form_validation->set_rules('register_username', 'username', 'trim|required|min_length[4]|max_length[14]|xss_clean|alpha_numeric');
  		$this->form_validation->set_rules('register_email', 'email', 'trim|required|valid_email|xss_clean');
		$this->form_validation->set_rules('register_password', 'password', 'trim|required|min_length[6]|xss_clean');
		$this->form_validation->set_rules('register_password_confirm', 'password confirmation', 'trim|required|matches[register_password]|xss_clean');

		$this->form_validation->set_error_delimiters('<img src="'.$this->template->page_url.'application/images/icons/exclamation.png" data-tip="', '" />');

		require_once('application/libraries/captcha.php');

		$captchaObj = new Captcha($this->config->item('use_captcha'));

		if(count($_POST))
		{
			$emailAvailable = $this->email_check($this->input->post('register_email'));
			$usernameAvailable = $this->username_check($this->input->post('register_username'));
		}
		else
		{
			$emailAvailable = false;
			$usernameAvailable = false;
		}

		//Check if everything went correct
		if($this->form_validation->run() == FALSE
			|| strtoupper($this->input->post('register_captcha')) != strtoupper($captchaObj->getValue())
			|| !count($_POST)
			|| !$usernameAvailable
			|| !$emailAvailable)
		{
			$fields = array('username', 'email', 'password', 'password_confirm');
			if($this->input->post("r") && $this->external_account_model->userExists($this->input->post("r")) == false)
			{
				$this->template->view($this->template->loadPage("page.tpl", array(
				"module" => "default", 
				"headline" => "Account creation",
				"content" => "Eror : the recruiter account doesn't exist !<br>Please register using the following link : <a href=\"register\">register</a>"
			)), false, "modules/register/js/validate.js", "Account Creation");
				//die("Error : The recruiter account doesn't exist !");
			}
			$recruiter = 0;
			if($this->input->post('r'))
				$recruiter = $this->input->post('r');
			if($this->input->get('r'))
				$recruiter = $this->input->get('r');
			$data = array(
						"username_error" => $this->usernameError,
						"email_error" => $this->emailError,
						"password_error" => "",
						"password_confirm_error" => "",
						"expansions" => $this->realms->getExpansions(),
						"use_captcha" => $this->config->item('use_captcha'),
						"captcha_error" => "",
						"recruiter" => $recruiter,
						"url" => $this->template->page_url
					);

			if(count($_POST) > 0)
			{
				// Loop through fields and assign error or success image
				foreach($fields as $field)
				{
					if(strlen(form_error('register_' . $field)) == 0 && empty($data[$field."_error"]))
					{
						$data[$field."_error"] = '<img src="'.$this->template->page_url.'application/images/icons/accept.png" />';
					}
					elseif(empty($data[$field."_error"]))
					{
						$data[$field."_error"] = form_error('register_' . $field);
					}
				}

				if($this->input->post('register_captcha') != $captchaObj->getValue())
				{
					$data['captcha_error'] = '<img src="'.$this->template->page_url.'application/images/icons/exclamation.png" />';
				}
			}

			// If not then display our page again
			$this->template->view($this->template->loadPage("page.tpl", array(
				"module" => "default", 
				"headline" => "Account creation", 
				"content" => $this->template->loadPage("register.tpl", $data),
			)), false, "modules/register/js/validate.js", "Account Creation");
		}
		else
		{
			if($this->input->post("register_expansion") != false)
			{
				if(!array_key_exists($this->input->post("register_expansion"), $this->realms->getExpansions()))
				{
					die("Hey, don't modify the expansion value...");
				}
			}

			if(!$this->username_check($this->input->post("register_username")))
			{
				die();
			}
			$recruiter = 0;
			if($this->input->post("r") != false)
			{
				$recruiter = $this->input->post("r");
			}
			$bridge = false;

			if($this->config->item('use_forum_bridge'))
			{
				if(file_exists('application/modules/register/bridges/'.$this->config->item('forum_bridge').".php"))
				{
					$bridge = true;
				}
				else
				{
					show_error("The forum bridge <b>".$this->config->item('forum_bridge')."</b> does not exist in <b>application/modules/register/controllers/bridges/</b>");
					die();
				}
			}

			if($this->config->item('enable_email_activation'))
			{
				$key = $this->activation_model->add($this->input->post('register_username'), $this->input->post('register_password'), $this->input->post('register_email'), $this->input->post('register_expansion'), $recruiter);
				
				$link = base_url().'register/activate/'.$key;

				$this->sendMail($this->input->post('register_email'), $this->config->item('activation_sender_mail'), $this->config->item('server_name').': activate account', 'You have created the account '.$this->input->post('register_username').', please go here to activate it: <a href="'.$link.'">'.$link.'</a>');
			}
			else
			{
				//Register our user.
				$this->external_account_model->createAccount($this->input->post('register_username'), $this->input->post('register_password'), $this->input->post('register_email'), $this->input->post('register_expansion'));
				$this->updateRecruiter($this->external_account_model->getId($this->input->post('register_username')), $recruiter);
				
				// Log in
				$sha_pass_hash = $this->user->createHash($this->input->post('register_username'), $this->input->post('register_password'));
				$check = $this->user->setUserDetails($this->input->post('register_username'), $sha_pass_hash);
			}

			// Show success message
			$data = array(
						"url" => $this->template->page_url,
						"account" => $this->input->post('register_username'),
						"bridgeName" => $this->config->item('forum_bridge'),
						"bridge" => $bridge,
						"username" => $this->input->post('register_username'),
						"email" => $this->input->post('register_email'),
						"password" => $this->input->post('register_password'),
						"email_activation" => $this->config->item('enable_email_activation')
					);

			$title = ($data['email_activation']) ? "Please confirm your account creation" : "Your account has been created!";

			$this->template->view($this->template->box($title, $this->template->loadPage("register_success.tpl", $data)));
		}
	}

	private function sendMail($receiver, $sender, $subject, $message)
	{
		$this->load->config('smtp');

		if($this->config->item('use_own_smtp_settings'))
		{
			$config['protocol'] = "smtp";
			$config['smtp_host'] = $this->config->item('smtp_host');
			$config['smtp_user'] = $this->config->item('smtp_user');
			$config['smtp_pass'] = $this->config->item('smtp_pass');
			$config['smtp_port'] = $this->config->item('smtp_port');
			$config['crlf'] = "\r\n";
			$config['newline'] = "\r\n";
		}

		$config['charset'] = 'utf-8';
		$config['wordwrap'] = TRUE;
		$config['mailtype'] = 'html';

		$this->load->library('email', $config);

		$this->email->from($sender, $this->config->item('server_name'));
		$this->email->to($receiver); 

		$this->email->subject($subject);
		$this->email->message($message);	

		$this->email->send();
	}
	
	public function email_check($email) 
	{
		if(!$this->external_account_model->emailExists($email))
		{
			$this->emailError = '';

			// The email does not exists so they can register
			return true;
		}
		else
		{
			// Email exists
			$this->emailError = '<img src="'.$this->template->page_url.'application/images/icons/exclamation.png" data-tip="This email is not available" />';
			
			return false;		
		}
	}
	
	public function username_check($username)
	{
		if(!$this->external_account_model->usernameExists($username))
		{
			$this->usernameError = '';

			// The user does not exists so they can register
			return true;
		}
		else
		{
			// User exists
			$this->usernameError = '<img src="'.$this->template->page_url.'application/images/icons/exclamation.png" data-tip="This username is not available" />';

			return false;		
		}
	}

	public function activate($key = false)
	{
		if(!$key)
		{
			$this->template->box("Invalid activation key", "The provided activation key appears to be invalid!", true);
		}

		$account = $this->activation_model->getAccount($key);

		if(!$account)
		{
			$this->template->box("Invalid activation key", "The provided activation key appears to be invalid!", true);
		}

		$this->activation_model->remove($account['id'], $account['username'], $account['email']);

		$this->external_account_model->createAccount($account['username'], $account['password'], $account['email'], $account['expansion'], true);
		$this->updateRecruiter($this->external_account_model->getId($account['username']), $account['recruiter']);
		
		// Log in
		$this->user->setUserDetails($account['username'], $account['password']);

		$bridge = false;

		if($this->config->item('use_forum_bridge'))
		{
			if(file_exists('application/modules/register/bridges/'.$this->config->item('forum_bridge').".php"))
			{
				$bridge = true;
			}
			else
			{
				show_error("The forum bridge <b>".$this->config->item('forum_bridge')."</b> does not exist in <b>application/modules/register/controllers/bridges/</b>");
				die();
			}
		}

		// Show success message
		$data = array(
			"url" => $this->template->page_url,
			"account" => $account['username'],
			"bridgeName" => $this->config->item('forum_bridge'),
			"bridge" => $bridge,
			"username" => $account['username'],
			"email" => $account['email'],
			"password" => $account['password'],
			"email_activation" => false
		);

		$title = "Your account has been created!";

		$this->template->view($this->template->box($title, $this->template->loadPage("register_success.tpl", $data)));
	}
	private function updateRecruiter($accountid, $recruiterid)
	{
		$auth = $this->external_account_model->getConnection();
		$query = "UPDATE account SET recruiter = ".$recruiterid." WHERE id = ".$accountid;
		$auth->query($query);
	}
}