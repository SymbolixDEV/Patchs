(function($) {
    var methods = {
        init: function(options) {
            methods.settings = $.extend({}, $.fn.ajaxListener.defaults, options);

            setTimeout(
                function() {
                    $('.alert')
                        .slideDown('medium')
                        .delay(methods.settings.hideDelayStatic)
                        .hide('fast')
                    ;
                },
                500
            );
			methods.listenIncomingResponses();
        },

        listenIncomingResponses: function() {
            $(document).ajaxComplete(function(event, xhr, settings) {
				try {
					var data = $.parseJSON(xhr.responseText);
				} catch(err) {
					return;
				}
				
				
				if (data.redirect) {
					window.location.href = data.redirect;
				}
				
				if (data.captchaupdate) {
					$(".wm-ui-captcha-img").attr('src', data.captchaupdate);
				}
				
                if (data.messages) {
                    var messages = data.messages;

                    var i;

                    if (messages.error) {
                        for (i = 0; i < messages.error.length; i++) {
                            methods.addError(messages.error[i]);
                        }
                    }

                    if (messages.success) {
                        for (i = 0; i < messages.success.length; i++) {
                            methods.addSuccess(messages.success[i]);
                        }
                    }

                    if (messages.info) {
                        for (i = 0; i < messages.info.length; i++) {
                            methods.addInfo(messages.info[i]);
                        }
                    }
                }
            });
        },

        addSuccess: function(message) {
            var flashMessageElt = methods.getBasicFlash(message).addClass('alert-success');

            methods.addToList(flashMessageElt);
            methods.display(flashMessageElt);
        },

        addError: function(message) {
            var flashMessageElt = methods.getBasicFlash(message).addClass('alert-error');

            methods.addToList(flashMessageElt);
            methods.display(flashMessageElt);
        },

        addInfo: function(message) {
            var flashMessageElt = methods.getBasicFlash(message).addClass('alert-info');

            methods.addToList(flashMessageElt);
            methods.display(flashMessageElt);
        },

        getBasicFlash: function(message) {
            var flashMessageElt = $('<div></div>')
                .hide()
                .addClass('alert')
                .append(methods.getCloseButton())
                .append($('<div></div>').html(message))
            ;

            return flashMessageElt;
        },

        getCloseButton: function() {
            var closeButtonElt = $('<button></button>')
                .addClass('close')
                .attr('data-dismiss', 'alert')
                .html('&times')
            ;

            return closeButtonElt;
        },

        addToList: function(flashMessageElt) {
            flashMessageElt.prependTo($('#wm-ui-flash-message'));
        },

        display: function(flashMessageElt) {
            setTimeout(
                function() {
                    flashMessageElt
                        .slideDown('medium')
                        .delay(methods.settings.hideDelay)
                        .hide('fast', function() { $(this).remove(); } )
                    ;
                },
                500
            );
        }
    };
	
    $.fn.ajaxListener = function(method) {
        if (methods[method]) {
            return methods[ method ].apply(this, Array.prototype.slice.call(arguments, 1));
        } else if (typeof method === 'object' || ! method) {
            return methods.init.apply(this, arguments);
        } else {
            $.error('Method ' +  method + ' does not exist on jQuery.flashNotification');
        }
    };

    $.fn.ajaxListener.defaults = {
        'hideDelay'         : 15000,
		'hideDelayStatic'   : 30000,
        'autoHide'          : true,
        'animate'           : true
    };
})(jQuery);

$(document).on('click','#wm-ui-flash-message .close',function() {
	$(this).parent().hide();
});