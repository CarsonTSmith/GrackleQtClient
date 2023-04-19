(function ($) {
    "use strict";

    
    /*==================================================================
    [ Validate ]*/
    var input = $('.validate-input .input100');

    $('.validate-form').on('submit',function(e) {
        e.preventDefault();
        var check = true;
        for (var i = 0; i < input.length; i++) {
            if (validate(input[i]) == false) {
                showValidate(input[i]);
                check=false;
            }
        }

        if (check === false) {
            return check;
        }

        // send ipc message to main with socket info and username
        let socket_info = {ip: document.getElementById('ip').value,
                           port: document.getElementById('port').value,
                           username: document.getElementById('username').value
        };
        window.electronAPI.cant_connect_to_server((event, function() {
            document.getElementById('cant-connect-txt').innerText = 'Failed to Connect';
            console.log('cant connect to server');
        }));
        
        window.electronAPI.connect_to_server(socket_info);
    });


    $('.validate-form .input100').each(function() {
        $(this).focus(function() {
           hideValidate(this);
        });
    });

    function validate (input) 
    {
        var ipformat   = /^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/;
        var portformat = /^([1-9][0-9]{0,3}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])$/;
        if($(input).attr('type') == 'ip' || $(input).attr('name') == 'ip') {
            if ($(input).val().trim().match(ipformat) == null) {
                console.log('ip failed');
                return false;
            }
        } else if ($(input).attr('type') == 'port' || $(input).attr('name') == 'port') {
            if ($(input).val().trim().match(portformat) == null) {
                console.log('port failed');
                return false;
            }
        } else if ($(input).attr('type') == 'username' || $(input).attr('name') == 'username') {
            if (($(input).val().length > 20) || ($(input).val().length < 3)) {
                console.log('username failed');
                return false;
            }
        }
    }

    function showValidate(input) 
    {
        var thisAlert = $(input).parent();

        $(thisAlert).addClass('alert-validate');
    }

    function hideValidate(input) {
        var thisAlert = $(input).parent();

        $(thisAlert).removeClass('alert-validate');
    }
    
})(jQuery);