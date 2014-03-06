var CharacterTransfer = {
	
	MODULE_NAME: 'character_transfer',
	
	// Languge strings
	LANG_SELECT_CHAR: 'Please select a character.',
	LANG_SELECT_DEST: 'Please enter destination account.',
	
	// Misc
	SelectedRealm: 0,
	SelectedCharacter: null,
	DestionationAccount: null,
	
	Initialize: function(realmId)
	{
		CharacterTransfer.SelectedRealm = realmId;
	},
	
	RedirectToSuccess: function()
	{
		// Redirect
		document.location = Config.URL + CharacterTransfer.MODULE_NAME + "/success/" + CharacterTransfer.SelectedRealm + '/' + CharacterTransfer.SelectedCharacter + '/' + CharacterTransfer.DestionationAccount;
	},
	
	RealmChange: function(select)
	{
		var selected = $(select).find(":selected");
		var value = selected.attr('value');
		
		if (typeof selected != 'undefined' && selected.length > 0 && typeof value != 'undefined' && value.length > 0)
		{
			// Set the new value
			CharacterTransfer.SelectedRealm = parseInt(value);
			
			// Disable the current character select
			$('#cta_visible_char_select').css('display', 'none');
			$('#cta_visible_char_select').attr('id', null);
			
			// Locate the character select for the newly selected realm
			// And enable it
			$('#cta_char_select_' + value).parent().css('display', 'block');
			$('#cta_char_select_' + value).parent().attr('id', 'cta_visible_char_select');
		}
		
		return true;
	},
	
	DestAccountUpdate: function(element)
	{
		var text = $(element).val();
		
		// Check the value
		if (typeof text != 'undefined' && text.length > 0)
		{
			CharacterTransfer.DestionationAccount = text;
		}
		
		return true;
	},
	
	Submit: function()
	{
		// Get the select character
		var Character = $('#cta_char_select_' + CharacterTransfer.SelectedRealm).find(":selected").attr('value');
		
		// Verify the character
		if (typeof Character == 'undefined' || Character.length == 0)
		{
			UI.alert(CharacterTransfer.LANG_SELECT_CHAR);
			return;
		}
		
		// Check the destination account
		if (CharacterTransfer.DestionationAccount == null)
		{
			UI.alert(CharacterTransfer.LANG_SELECT_DEST);
			return;
		}
		
		// Save the character
		CharacterTransfer.SelectedCharacter = Character;
		
		// Set a little loading message
		UI.alert('Loading...');
		
		// Process the transfer
		$.post(Config.URL + CharacterTransfer.MODULE_NAME + "/process", 
			{
				csrf_token_name: Config.CSRF,
				realmId: CharacterTransfer.SelectedRealm,
				character: Character,
				destination: CharacterTransfer.DestionationAccount
				
			},
			function(data)
			{
				// Check for errors
				if (data != 'OK')
				{
					UI.alert(data);
				}
				else
				{
					CharacterTransfer.RedirectToSuccess();
				}
				return;
			}, 
			"html"
		);
		
		return true;
	}
}