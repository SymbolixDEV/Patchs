var CTA = {
	
	MODULE_NAME: 'character_transfer',
	
	search: function(type)
	{
		var string = $("#search").val();
		
		$("#logs_list").html('<center><img src="' + Config.URL + 'application/themes/admin/images/ajax.gif" /><br /><br /></center>');
	
		$.post(Config.URL + CTA.MODULE_NAME + "/admin/search", {string: string, csrf_token_name: Config.CSRF}, function(data)
		{
			$("#logs_list").fadeOut(150, function()
			{
				$(this).html(data).slideDown(500, function()
				{
					Tooltip.refresh();
				});
			});
		});	
	}
};