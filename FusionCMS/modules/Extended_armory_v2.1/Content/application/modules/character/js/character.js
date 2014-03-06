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
	 getIcon: function(id, realm, slot)
	 {
	 	if($.inArray(id, this.loadedIcons) == -1)
	 	{
	 		this.loadedIcons.push(id);

	 		$.get(Config.URL + "icon/get/" + realm + "/" + id, function(data)
		 	{
		 		$(".get_icon_" + id).each(function()
		 		{
		 			$(this).html("<div class='item'><a href='" + Config.URL + "item/" + realm + "/" + id + "' "+(Config.UseFusionTooltip ? 'rel-e' : 'rel')+"='item=" + id + "' data-item-slot='" + slot + "'  data-realm='" + realm + "'></a><img src='https://wow.zamimg.com/images/wow/icons/large/" + data + ".jpg' /></div>");
		 			TooltipExtended.refresh();
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
		
		Tooltip.show('Loading...');
		
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
		
		Tooltip.show('Loading...');
		
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

Number.prototype.hasFlag = function(flag)
{
    return (this & parseInt(flag)) === parseInt(flag);
}

var SOCKET_COLOR_META = 1;
var SOCKET_COLOR_RED = 2;
var SOCKET_COLOR_YELLOW = 4;
var SOCKET_COLOR_BLUE = 8;

/**
 * Extended Tooltip related functions
 */
function TooltipExtended()
{
	/**
	 * Add event-listeners
	 */
	this.initialize = function()
	{
		// Add mouse-over event listeners
		this.addEvents();
	}

	this.addEvents = function()
	{
		if (Config.UseFusionTooltip)
		{
			$("[rel-e]").hover(
				function()
				{
					if(/^item=[0-9]*$/.test($(this).attr("rel-e")))
					{
						TooltipExtended.Item.get(this, function(data)
						{
							TooltipExtended.show(data);
						});
					}
				},
				function()
				{
					$("#tooltip").hide();
					
					if (TooltipExtended.Item.currentAjax != null)
						TooltipExtended.Item.currentAjax.abort();
				}
			);
		}
	}

	/**
	 * Used to support Ajax content
	 * Reloads the tooltip elements
	 */
	this.refresh = function()
	{
		// Re-add
		this.addEvents();
	}
	
	/**
	 * Displays the tooltip
	 * @param Object element
	 */
	this.show = function(data)
	{
		$("#tooltip").html(data).show();
	}
	
	this.AllGems = false;
	
	this.GetAllPlayerGems = function()
	{
		if (this.AllGems)
			return this.AllGems;
		
		var AllGems = new Array()
		AllGems[SOCKET_COLOR_META] = [];
		AllGems[SOCKET_COLOR_RED] = [];
		AllGems[SOCKET_COLOR_YELLOW] = [];
		AllGems[SOCKET_COLOR_BLUE] = [];
		
		$.each(TooltipPlayerData, function(slot, item)
		{
			var Gems = item.gems;
			
			if (Gems)
			{
				$.each(Gems, function(i, gem)
				{
					var GemColor = parseInt(gem.color);
					
					if (GemColor.hasFlag(SOCKET_COLOR_META))
					{
						AllGems[SOCKET_COLOR_META].push(gem);
					}
					
					if (GemColor.hasFlag(SOCKET_COLOR_RED))
					{
						AllGems[SOCKET_COLOR_RED].push(gem);
					}
					
					if (GemColor.hasFlag(SOCKET_COLOR_YELLOW))
					{
						AllGems[SOCKET_COLOR_YELLOW].push(gem);
					}
					
					if (GemColor.hasFlag(SOCKET_COLOR_BLUE))
					{
						AllGems[SOCKET_COLOR_BLUE].push(gem);
					}
				});
			}
		});
		
		//save
		this.AllGems = AllGems;
		
		//return
		return this.AllGems;
	};
	
	/**
	 * Item tooltip object
	 */
	 this.Item = new function()
	 {
	 	/**
	 	 * Loading HTML
	 	 */
	 	this.loading = "Loading...";

	 	/**
	 	 * The currently displayed item ID
	 	 */
	 	this.currentId = false;
		
		/**
		 * Used to interrupt the ajax in progress on mouse out
		 */
		this.currentAjax = null;
		
		/**
		 * Runtime cache
		 */
		this.cache = {
			gems: new Array(),
			spells: new Array()
		};
		
		this.GemColorString = function(id)
		{
			switch (parseInt(id))
			{
				case 1: return 'Meta';
				case 2: return 'Red';
				case 4: return 'Yellow';
				case 8: return 'Blue';
			}
			
			return 'Unknown ' + id;
		};
		
	 	/**
	 	 * Load an item and display it in the tooltip
	 	 * @param Object element
	 	 * @param Function callback
	 	 */
	 	this.get = function(element, callback)
	 	{
	 		var obj = $(element);
	 		var realm = obj.attr("data-realm");
	 		var id = obj.attr("rel-e").replace("item=", "");
			
			var slot = parseInt(obj.attr("data-item-slot"));
			
			//try getting the visit cache
			var cache = $.data(element, 'tooltip-cache');
			
	 		TooltipExtended.Item.currentId = id;
			
	 		if (typeof cache != 'undefined')
	 		{
	 			callback(cache)
	 		}
	 		else
	 		{
				callback(this.loading);
				
				var RequiredGems = 0;
				var MatchedGems = 0;
				
				TooltipExtended.Item.currentAjax = $.get(Config.URL + "tooltip/" + realm + "/" + id, function(data)
				{
					data = $(data);
					
					//Check required matching sockets for sock bonus
					data.find('.socket-slot').each(function(i, s)
					{
						if ($(s).hasClass('socket-required'))
						{
							//increase the required gems count
							RequiredGems++;
						}
					});
					
					//handle item player data
					if (typeof TooltipPlayerData != 'undefined' && (slot in TooltipPlayerData))
					{
						var ItemPlayerData = TooltipPlayerData[slot];
						
						//handle enchants
						if (ItemPlayerData.enchant)
						{
							data.find('#tooltip-item-enchantments').html(ItemPlayerData.enchant.description);
						}
						
						//handle extra sockets
						if (ItemPlayerData.hasExtraSocket)
						{
							data.find('#tooltip-item-sockets').append('<span class="socket-slot socket-prismatic q0">Prismatic Socket</span>');
						}
						
						//handle gems
						if (ItemPlayerData.gems)
						{
							//loop the gems
							$.each(ItemPlayerData.gems, function(key, gem)
							{
								var GemPosition = gem.slot;
								
								data.find('.socket-slot').each(function(i, s)
								{
									if (i == GemPosition)
									{
										//Set the gem text
										$(s).html(gem.text);
										
										//Metas should be activated only once checked
										if (parseInt(gem.color) != SOCKET_COLOR_META)
											$(s).addClass('q1');
										
										//Check if we have an icon
										if (typeof gem.icon == 'string')
										{
											$(s).css('background-image', 'url(http://wow.zamimg.com/images/wow/icons/tiny/'+gem.icon+'.gif)');
										}
										else
										{
											//Check the runtine cache
											if (typeof TooltipExtended.Item.cache.gems[gem.GemID] == 'undefined')
											{
												//Finally if no icon was found, pull one
												$.get(Config.URL + "icon/get/" + realm + "/" + gem.GemID, function(gemIconData)
												{
													$(s).css('background-image', 'url(http://wow.zamimg.com/images/wow/icons/tiny/'+gemIconData+'.gif)');
													//save to cache
													TooltipExtended.Item.cache.gems[gem.GemID] = gemIconData;
												});
											}
											else
											{
												$(s).css('background-image', 'url(http://wow.zamimg.com/images/wow/icons/tiny/'+TooltipExtended.Item.cache.gems[gem.GemID]+'.gif)');
											}
										}
										
										//Check if the socket is required for socket bonus
										if ($(s).hasClass('socket-required'))
										{
											var SocketColor = parseInt($(s).attr('data-socket-color'));
											var GemColor = parseInt(gem.color);
											//check if this game matches this socket
											if (GemColor.hasFlag(SocketColor))
											{
												MatchedGems++;
											}
										}
										
										//Check for meta requiries
										if (parseInt(gem.color) == SOCKET_COLOR_META)
										{
											var TotalGemRequierementsMet = 0;
											
											if (gem.requires)
											{
												//Reverse the array order
												var Requiries = gem.requires.reverse();
												
												//Fetch all the gems
												var AllGems = TooltipExtended.GetAllPlayerGems();
												
												$.each(Requiries, function(kk, req)
												{
													var text;
													var isActive = false;
													
													//Switch between the diferent comparators
													switch (parseInt(req.comparator))
													{
														case 2:
															text = 'Requires less ' + TooltipExtended.Item.GemColorString(req.color) +  ' gems than ' + TooltipExtended.Item.GemColorString(req.compareColor) + ' gems';
															if (AllGems[parseInt(req.color)].length < AllGems[parseInt(req.compareColor)].length)
															{
																isActive = true;
																TotalGemRequierementsMet++;
															}
															break;
														case 3:
															text = 'Requires more ' + TooltipExtended.Item.GemColorString(req.color) +  ' gems than ' + TooltipExtended.Item.GemColorString(req.compareColor) + ' gems';
															if (AllGems[parseInt(req.color)].length > AllGems[parseInt(req.compareColor)].length)
															{
																isActive = true;
																TotalGemRequierementsMet++;
															}
															break;
														case 5:
															text = 'Requires at least ' + req.count + ' ' + TooltipExtended.Item.GemColorString(req.color) +  ' gem' + (req.count > 1 ? 's' : '');
															if (AllGems[parseInt(req.color)].length >= parseInt(req.count))
															{
																isActive = true;
																TotalGemRequierementsMet++;
															}
															break;
													}
													
													//Append the text
													$(s).after('<br /><span class="meta-socket-requires ' + (isActive ? 'q1' : 'q0') + '">' + text + '</span>');
												});
											}
											
											//Check if all reqs are met
											if (!gem.requires || TotalGemRequierementsMet == gem.requires.length)
											{
												$(s).addClass('q1').removeClass('q0');
											}
										}
									}
								});
							});
						}
						
						//handle sock bonus
						if (RequiredGems > 0)
						{
							if (MatchedGems >= RequiredGems)
								data.find('#tooltip-item-sock-bonus').addClass('q2');
						}
					}
					
					//handle active item set pieces
					if (typeof TooltipEquippedItems != 'undefined')
					{
						//loop trough the itemset pieces
						data.find('.item-set-piece').each(function(i, e)
						{
							var possibleEntriesString = $(e).attr('data-possible-entries');
							//split into array
							var possibleEntries = new Array();
							//make sure we have more than 1 entry
							if (possibleEntriesString.indexOf(':') > -1)
							{
								possibleEntries = possibleEntriesString.split(':');
							}
							else
							{
								possibleEntries[0] = possibleEntriesString;
							}
							//loop the possible entries and check if one of the is equipped
							$.each(possibleEntries, function(i2, v2)
							{
								if ($.inArray(parseInt(v2), TooltipEquippedItems) > -1)
								{
									//active the piece
									$(e).addClass('q8');
									$(e).addClass('item-set-active-piece');
								}
							});
						});
						//get the active pieces count
						var activePiecesCount = data.find('.item-set-active-piece').length;
						//update the equipped item set pieces count
						if (data.find('#tooltip-item-set-count').length > 0)
						{
							data.find('#tooltip-item-set-count').html(activePiecesCount);
						}
						//update the set bonuses
						if (activePiecesCount > 0)
						{
							data.find('.item-set-bonus').each(function(i, e)
							{
								var requiredPieces = $(e).attr('data-bonus-required-items');
								//activate the set bonus
								if (activePiecesCount >= requiredPieces)
								{
									$(e).addClass('q2');
								}
							});
						}
					}
					
					// Cache it this visit
					$.data(element, 'tooltip-cache', data);
					
					// Make sure it's still visible
					if($("#tooltip").is(":visible") && TooltipExtended.Item.currentId == id)
					{
						callback(data);
					}
				});
		 	}
	 	}
	 }
}

var TooltipExtended = new TooltipExtended();

//initialize the extended tooltip
TooltipExtended.initialize();