$(function() {
    if($('meta[name=csrf-token]').length > 0) {
	    $.ajaxPrefilter(function (options, originalOptions, jqXHR) {
	    	jqXHR.setRequestHeader('X-CSRF-Token', $('meta[name=csrf-token]').attr('content'));
	    });

	    $('#wm-ui-flash-message').ajaxListener('init');
    }
});

(function($) {
    var methods = {
        init: function(options) {
            methods.settings = $.extend({}, $.warmane.defaults, options);	
            methods.setupRotators();
        },
        
        resolveLocation: function(ip, ret, ele) {
            $.get('https://freegeoip.net/json/' + ip, function(response) {
                if(ret) {
                    return response;
                } else {
                    $(ele).html(response.country_name);
                }
            }, "jsonp"); 
        },
        
        initAlerts: function() {
            setInterval(function() {
                $.ajax({
                    type: "GET",
                    url: "/?alert&t=" + methods.settings.alertTime,
                    dataType:"json",
                    success:function(data) {
                        methods.settings.alertTime = data.time;
                        if(data.alerts) {
                            $.titleAlert("Warning Message", {
                                requireBlur:false,
                                stopOnFocus:true,
                                duration:30000,
                                interval:700
                            });
                            $.each(data.alerts, function(index, value) {
                                $.gritter.add({
                                    title: value.title,
                                    text: value.text,
                                    time: 60000,
                                });
                            });
                        }
                    }
                });
            }, 30000);
        },
        
        setupRotators: function() {
            console.log('Calling global warmane backgrounds.');
            $.ajax({ 
                type: 'GET', 
                url: '//www.warmane.com/?global',
                dataType: 'json',
                success: function (result) {
                    console.log('Got warmane backtrounds.');
                    methods.settings.backgrounds = result;
                    $.each(methods.settings.backgrounds, function(index, value) {
                        $('<a>')
                        .attr('href','javascript:;')
                        .attr('data-background',index)
                        .prependTo('#wm-theme-navigation');
                    });
                    
                    methods.rotateBackgrounds();
                    methods.forceBackground();
                },
                error: function(result) {
                    console.log(result);
                }
            });
        },
        
        forceBackground: function() {
            $('a[data-background]').on('click', function() {
                methods.settings.forceBackground = $(this).attr('data-background');
                methods.rotateBackgrounds();
            })
        },
  
        rotateBackgrounds: function() {
            clearTimeout(methods.settings.backgroundTimer);

            if(methods.settings.currentBackground > -1 && methods.settings.backgrounds[methods.settings.currentBackground]['render'].length > 1) {
                $(methods.settings.backgrounds[methods.settings.currentBackground]['render']).fadeOut(250);
            }
            methods.settings.currentBackground++;

            if(methods.settings.forceBackground != -1) {
                methods.settings.currentBackground = methods.settings.forceBackground;
            }

            if(methods.settings.currentBackground > 1) {
                methods.settings.currentBackground = 0;
            }
            $('.bg-rotator').fadeOut(250, function() {
                $('.bg-rotator').css({
                    'background-image' : "url('" + methods.settings.backgrounds[methods.settings.currentBackground]['image'] + "')"
                })
                if(methods.settings.backgrounds[methods.settings.currentBackground]['url'].length > 1) {
                    $('.bg-rotator')
                    .css({'cursor' : 'pointer'})
                    .attr('onclick','location.href="' + methods.settings.backgrounds[methods.settings.currentBackground]['url'] + '";');
                } else {
                    $('.bg-rotator')
                    .css({'cursor' : ''})
                    .attr('onclick','');
                }
                
                if(methods.settings.backgrounds[methods.settings.currentBackground]['render'].length > 1) {
                    $(methods.settings.backgrounds[methods.settings.currentBackground]['render']).fadeIn(250);
                }
                                    
                $('.bg-rotator').fadeIn(250);
            });
            
            if(methods.settings.forceBackground == -1) {
                //methods.settings.backgroundTimer = setTimeout(methods.rotateBackgrounds, methods.settings.backgrounds[methods.settings.currentBackground]['timeout']); 
            }
        },
        
		showTooltip: function(event, text, icon) {
		    $oWoWTooltip.showTooltip(event, text, icon, 4)
		},
		
		hideTooltip: function() {
			$oWoWTooltip.hideTooltip();
		},
		
		moveTooltip: function(event) {
			$oWoWTooltip.moveTooltip(event)
		}
    };
    
    $.warmane = function(method) {
        if (methods[method]) {
            return methods[ method ].apply(this, Array.prototype.slice.call(arguments, 1));
        } else if (typeof method === 'object' || ! method) {
            return methods.init.apply(this, arguments);
        } else {
            $.error('Method ' +  method + ' does not exist on jQuery.warmane');
        }
    };
    
    $.warmane.defaults =  {
        backgroundTimer: null,
        forceBackground: -1
	};
})(jQuery);

function in_array(needle, haystack) {
    for(var i in haystack) {
        if(haystack[i] == needle) return true;
    }
    return false;
}

function is_numeric(n) {
    return !isNaN(parseFloat(n)) && isFinite(n);
}

(function($) {
	$.fn.equalizeCols = function() {
        var height = 0,
            reset = "auto";
  
        return this
            .css("height", reset)
            .each(function() {
                height = Math.max(height, this.offsetHeight);
            })
            .css("height", height)
            .each(function() {
                var h = this.offsetHeight;
                if (h > height) {
                    $(this).css("height", height - (h - height));
                };
            });	
        };
})(jQuery);

;(function($){	
	$.titleAlert = function(text, settings) {
		if ($.titleAlert._running)
			$.titleAlert.stop();

		$.titleAlert._settings = settings = $.extend( {}, $.titleAlert.defaults, settings);

		if (settings.requireBlur && $.titleAlert.hasFocus)
			return;

		settings.originalTitleInterval = settings.originalTitleInterval || settings.interval;
		
		$.titleAlert._running = true;
		$.titleAlert._initialText = document.title;
		document.title = text;
		var showingAlertTitle = true;
		var switchTitle = function() {
			if (!$.titleAlert._running)
				return;
			
		    showingAlertTitle = !showingAlertTitle;
		    document.title = (showingAlertTitle ? text : $.titleAlert._initialText);
		    $.titleAlert._intervalToken = setTimeout(switchTitle, (showingAlertTitle ? settings.interval : settings.originalTitleInterval));
		}
		$.titleAlert._intervalToken = setTimeout(switchTitle, settings.interval);
		
		if (settings.stopOnMouseMove) {
			$(document).mousemove(function(event) {
				$(this).unbind(event);
				$.titleAlert.stop();
			});
		}
		
		if (settings.duration > 0) {
			$.titleAlert._timeoutToken = setTimeout(function() {
				$.titleAlert.stop();
			}, settings.duration);
		}
	};

	$.titleAlert.defaults = {
		interval: 500,
		originalTitleInterval: null,
		duration:0,
		stopOnFocus: true,
		requireBlur: false,
		stopOnMouseMove: false
	};

	$.titleAlert.stop = function() {
		if (!$.titleAlert._running)
			return;
		
		clearTimeout($.titleAlert._intervalToken);
		clearTimeout($.titleAlert._timeoutToken);
		document.title = $.titleAlert._initialText;
		
		$.titleAlert._timeoutToken = null;
		$.titleAlert._intervalToken = null;
		$.titleAlert._initialText = null;
		$.titleAlert._running = false;
		$.titleAlert._settings = null;
	}
	
	$.titleAlert.hasFocus = true;
	$.titleAlert._running = false;
	$.titleAlert._intervalToken = null;
	$.titleAlert._timeoutToken = null;
	$.titleAlert._initialText = null;
	$.titleAlert._settings = null;
	
	
	$.titleAlert._focus = function () {
		$.titleAlert.hasFocus = true;
		
		if ($.titleAlert._running && $.titleAlert._settings.stopOnFocus) {
			var initialText = $.titleAlert._initialText;
			$.titleAlert.stop();
			
			setTimeout(function() {
				if ($.titleAlert._running)
					return;
				document.title = ".";
				document.title = initialText;
			}, 1000);
		}
	};
	$.titleAlert._blur = function () {
		$.titleAlert.hasFocus = false;
	};

	$(window).bind("focus", $.titleAlert._focus);
	$(window).bind("blur", $.titleAlert._blur);
})(jQuery);