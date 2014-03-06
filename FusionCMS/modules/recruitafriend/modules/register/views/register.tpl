{form_open('register', 'class="page_form"')}
	<table style="width:80%">
		<tr>
			<td><label for="register_username">Username</label></td>
			<td>
				<input type="text" name="register_username" id="register_username" value="{set_value('register_username')}" onChange="Validate.checkUsername()"/>
				<span id="username_error">{$username_error}</span>
			</td>
		</tr>
		<tr>
			<td><label for="register_email">Email</label></td>
			<td>
				<input type="email" name="register_email" id="register_email" value="{set_value('register_email')}" onChange="Validate.checkEmail()"/>
				<span id="email_error">{$email_error}</span>
			</td>
		</tr>
		<tr>
			<td><label for="register_password">Password</label></td>
			<td>
				<input type="password" name="register_password" id="register_password" value="{set_value('register_password')}" onChange="Validate.checkPassword()"/>
				<span id="password_error">{$password_error}</span>
			</td>
		</tr>
		<tr>
			<td><label for="register_password_confirm">Confirm password</label></td>
			<td>
				<input type="password" name="register_password_confirm" id="register_password_confirm" value="{set_value('register_password_confirm')}" onChange="Validate.checkPasswordConfirm()"/>
				<span id="password_confirm_error">{$password_confirm_error}</span>
			</td>
		</tr>
		<tr>
			<td><label for="register_expansion">Expansion</label></td>
			<td>
				<select id="register_expansion" name="register_expansion">
					{foreach from=$expansions key=id item=expansion}
						<option value="{$id}">{$expansion}</option>
					{/foreach}
				</select>
			</td>
		</tr>

		{if $use_captcha}
			<tr>
				<td><label for="captcha"><img src="{$url}application/modules/register/controllers/getCaptcha.php?{uniqid()}" /></label></td>
				<td>
					<input type="text" name="register_captcha" id="register_captcha"/>
					
					<span id="captcha_error">{$captcha_error}</span>
				</td>
			</tr>
		{/if}
		<input type="hidden" name="r" value="{$recruiter}"/>
	</table>
	<center style="margin-bottom:10px;">
		<input type="submit" name="login_submit" value="Create account!" />
	</center>
{form_close()}