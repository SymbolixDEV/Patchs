var Character = {
	
	loadedIcons: [],
	loadedTalents: [],
	loadedGlyphs: [],
	currentAjax: null,
	/**
	 * Performs an ajax call to get the display name
	 * This should only be called if the icon cache was empty
	 * @param Int id
	 */
	 getIcon: function(id, realm)
	 {
	 	if($.inArray(id, this.loadedIcons) == -1)
	 	{
	 		this.loadedIcons.push(id);

	 		$.get(Config.URL + "icon/get/" + realm + "/" + id, function(data)
		 	{
		 		$(".get_icon_" + id).each(function()
		 		{
		 			$(this).html("<div class='item'><a href='" + Config.URL + "item/" + realm + "/" + id + "' rel='item=" + id + "' data-realm='" + realm + "'></a><img src='https://wow.zamimg.com/images/wow/icons/large/" + data + ".jpg' /></div>");
		 			Tooltip.refresh();
		 		});
		 	});
	 	}
	 },

	 /**
	  * Whether the tabs are changing or not
	  * @type Boolean
	  */
	 tabsAreChanging: false,

	 /**
	  * Change tab
	  * @param String selector
	  * @param Object link
	  */
	 tab: function(selector, link)
	 {
	 	if(!this.tabsAreChanging)
	 	{
	 		this.tabsAreChanging = true;

		 	// Find out the current tab
		 	var currentTabLink = $(".armory_current_tab");
		 	var currentTabId = "#tab_" + currentTabLink.attr("onClick").replace("Character.tab('", "").replace("', this)", "");

		 	// Change link states
		 	currentTabLink.removeClass("armory_current_tab");
		 	$(link).addClass("armory_current_tab");

		 	// Fade the current and show the new
		 	$(currentTabId).fadeOut(300, function()
		 	{
		 		$("#tab_" + selector).fadeIn(300, function()
	 			{
	 				Character.tabsAreChanging = false;
	 			});
		 	});
	 	}
	 },

	 /**
	  * Slide to an attributes tab
	  * @param Int id
	  */
	 attributes: function(id)
	 {
	 	if(id == 2)
	 	{
	 		$("#attributes_wrapper").animate({marginLeft:"-367px"}, 500);
	 		$("#tab_armory_1").fadeTo(500, 0.1);
	 		$("#tab_armory_3").fadeTo(500, 0.1);
	 		$("#tab_armory_2").fadeTo(500, 1);
	 	}
	 	else if(id == 1)
	 	{
	 		$("#attributes_wrapper").animate({marginLeft:"0px"}, 500);
	 		$("#tab_armory_2").fadeTo(500, 0.1);
	 		$("#tab_armory_3").fadeTo(500, 0.1);
	 		$("#tab_armory_1").fadeTo(500, 1);
	 	}
	 	else
	 	{
	 		$("#attributes_wrapper").animate({marginLeft:"-734px"}, 500);
	 		$("#tab_armory_1").fadeTo(500, 0.1);
	 		$("#tab_armory_2").fadeTo(500, 0.1);
	 		$("#tab_armory_3").fadeTo(500, 1);
	 	}
	 }
}

/**************** TALENTS *************************/

$('.talents-spec').click(function()
{
	if ($(this).hasClass('talents-spec-active'))
		return false;
	
	var $tabId = $(this).attr('specId');
	
	if ($tabId.length == 0)
		return;
	
	//disable the currenly selected one
	$('.talents-spec').each(function(index, element)
	{
        if ($(element).hasClass('talents-spec-active'))
		{
			//the active is found
			var activeTabId = $(element).attr('specId');
			//remove the class
			$(element).removeClass('talents-spec-active');
			//hide the talents table and crap
			$('.talents[specId="'+activeTabId+'"]').hide();
		}
    });
	
	//enable the new tab
	$(this).addClass('talents-spec-active')
	$('.talents[specId="'+$tabId+'"]').show();
});

$("[data-tip-talent]").hover(
	function()
	{
		var $spellId = $(this).attr("data-tip-talent");
		var $realmId = $(this).attr("data-tip-realm");
		var $rank = $(this).attr("data-tip-rank");
		var $max = $(this).attr("data-tip-max");
		
		if (typeof Character.loadedTalents[$spellId] == 'undefined')
		{
			Character.currentAjax = $.getJSON(Config.URL + "spell_data/index/" + $realmId + "/" + $spellId + "/talent", function(data)
			{
				data.tooltip = $(data.tooltip);
				//hide the rank column
				data.tooltip.find('#spell-tooltip-rank').css('display', 'none');
				//append the talnet rank
				data.tooltip.find('#spell-tooltip-name').after('<br><b>Rank '+$rank+'/'+$max+'</b>');
				
				//save as loaded
				Character.loadedTalents[$spellId] = data;
				
				Tooltip.show(data.tooltip);
			});
		}
		else
		{
			var data = Character.loadedTalents[$spellId];
			
			Tooltip.show(data.tooltip);
		}
	},
	function()
	{
		$("#tooltip").hide();
		
		if (Character.currentAjax != null)
			Character.currentAjax.abort();
		
		Character.currentAjax = null;
	}
);

$("[data-tip-glyph]").hover(
	function()
	{
		var $glyphId = parseInt($(this).attr("data-tip-glyph"));
		var $type = $(this).attr("data-tip-type");
		var $realmId = $(this).attr("data-tip-realm");
		
		if ($glyphId > 0)
		{
			if (typeof Character.loadedGlyphs[$glyphId] == 'undefined')
			{
				Character.currentAjax = $.getJSON(Config.URL + "spell_data/index/" + $realmId + "/" + $glyphId + "/glyph", function(data)
				{
					//save as loaded
					Character.loadedGlyphs[$glyphId] = data;
					
					Tooltip.show('<table>' +
									'<tbody>' +
										'<tr>' +
											'<td style="max-width: 320px;">' +
												'<b style="font-size: 15px;">' + data.name + '</b><br>' +
												'<span style="color: #71D5FF;">'+$type+'</span><br>' +
												'<span class="q">' + data.tooltip + '</span><br>' +		
											'</td>' +
										'</tr>' +
									'</tbody>' +
								'</table>');
				});
			}
			else
			{
				var data = Character.loadedGlyphs[$glyphId];
				
				Tooltip.show('<table>' +
									'<tbody>' +
										'<tr>' +
											'<td style="max-width: 320px;">' +
												'<b style="font-size: 15px;">' + data.name + '</b><br>' +
												'<span style="color: #71D5FF;">'+$type+'</span><br>' +
												'<span class="q">' + data.tooltip + '</span><br>' +		
											'</td>' +
										'</tr>' +
									'</tbody>' +
								'</table>');
			}
		}
		else
		{
			Tooltip.show('<b style="font-size: 15px;">Empty</b><br /><span style="color: #71D5FF;">'+$type+'</span>');
		}
	},
	function()
	{
		$("#tooltip").hide();
		
		if (Character.currentAjax != null)
			Character.currentAjax.abort();
		
		Character.currentAjax = null;
	}
);