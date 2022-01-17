// Supports Settings Page

var posted = false;
var unloadingState = false;
var loaded = 0; // Hold data load status
var numReq = 2; // Number of JSON required

// Tab tracking
var previousTab = "";
var currentTab = "";

// Handle unloading page while making a getJSON call
$(window).bind("beforeunload", function() {
    unloadingState = true;
});

function loadHash() { // Link to tab via hash value
    var url = document.location.toString();
    if (url.match('#')) {
        $('.nav-tabs a[href="#' + url.split('#')[1] + '"]').tab('show');
    }

    // Change hash for page-reload
    $('.nav-tabs a').on('shown.bs.tab', function(e) {
        window.location.hash = e.target.hash;
    });
}

function populatePage() {
    $(document).tooltip({
        'selector': '[data-toggle=tooltip]',
        'toggleEnabled': true
    });
    loadHash();
    populateForm();
    loadTab();
    pollComplete();
}

function loadTab() {
    // Javascript to enable linking to tab
    var url = document.location.toString();
    if (url.match('#')) {
        $('.nav-tabs a[href="#' + url.split('#')[1] + '"]').tab('show');
    }

    // Change hash for page-reload
    $('.nav-tabs a').on('shown.bs.tab', function(e) {
        window.location.hash = e.target.hash;
    });
}

function populateForm() { // Get current parameters
    var url = "/config/";
    var config = $.getJSON(url, function() {})
        .done(function(config) {
            try {
                $('#mdnsid').val(config.hostname);
                $('#ispindhubname').val(config.ispindhub.name);
                $('#urltargeturl').val(config.urltarget.url);
                $('#urlfreq').val(config.urltarget.freq);
                $('#brewersfriendkey').val(config.brewersfriend.key);
                $('#brewersfriendfreq').val(config.brewersfriend.freq);
                $('#brewfatherkey').val(config.brewfather.key);
                $('#brewfatherfreq').val(config.brewfather.freq);
            } catch {
                if (!unloadingState) {
                    alert("Unable to parse configuration data.");
                }
            }
        })
        .fail(function() {
            if (!unloadingState) {
                alert("Unable to retrieve configuration data.");
            }
        })
        .always(function() {
            // Can post-process here
        });
}


function pollComplete() { // Poll to see if entire page is loaded
    if (loaded == numReq) {
        posted = true;
        // finishPage();
    } else {
        setTimeout(pollComplete, 300); // try again in 300 milliseconds
    }
}

// POST Handlers:

function processPost(obj) {
    posted = false;
    event.preventDefault();
    hashLoc = window.location.hash;
    console.log(hashLoc);
    var $form = $(obj);
    url = $form.attr("action");

    $("button[id='submitSettings']").prop('disabled', true);
    $("button[id='submitSettings']").html('<i class="fa fa-spinner fa-spin"></i> Updating');

    // Switch here for hashLoc
    switch (hashLoc) {
        case "#ispindhub":
            processiSpindHubPost(url, obj);
            break;
        case "#temperature":
            processTemperaturePost(url, obj);
            break;
        case "#urltarget":
            processURLTargetPost(url, obj);
            break;
        case "#brewpiles":
            processBPiLessPost(url, obj);
            break;
        case "#brewersfriend":
            processBrewersFriendPost(url, obj);
            break;
        case "#brewfather":
            processBrewfatherPost(url, obj);
            break;
        case "#thingspeak":
            processThingSpeakPost(url, obj, 8);
            break;
        default:
            // Unknown hash location passed
            break;
    }
    buttonClearDelay();
}

function buttonClearDelay() { // Poll to see if entire page is loaded
    if (posted) {
        $("button[id='submitSettings']").prop('disabled', false);
        $("button[id='submitSettings']").html('Update');
        posted = false;
    } else {
        setTimeout(buttonClearDelay, 500); // try again in 300 milliseconds
    }
}

function processiSpindHubPost(url, obj) {
    // Handle Controller settings posts

    // Get form data
    var $form = $(obj),
        ispindhubname = $form.find("input[name='ispindhubname']").val();
        ispindhubTZ = $form.find("select[name='ispindhubTZ'").val();

    // Process post
    data = {
        ispindhubname: ispindhubname,
        ispindhubTZ : ispindhubTZ
    };
    postData(url, data);
}


function processURLTargetPost(url, obj) {
    // Handle URL target posts

    // Get form data
    var $form = $(obj),
        urltargeturl = $form.find("input[name='urltargeturl']").val(),
        urlfreq = $form.find("input[name='urlfreq']").val();

    // Process post
    data = {
        urltargeturl: urltargeturl,
        urlfreq: urlfreq
    };
    postData(url, data);
}

function processBPiLessPost(url, obj) {
    // Handle URL target posts

    // Get form data
    var $form = $(obj),
        urltargeturl = $form.find("input[name='bpilessurl']").val(),
        urlfreq = $form.find("input[name='bpilessfreq']").val();

    // Process post
    data = {
        urltargeturl: urltargeturl,
        urlfreq: urlfreq
    };
    postData(url, data);
}

function processBrewersFriendPost(url, obj) {
    // Handle Brewer's Friend target posts

    // Get form data
    var $form = $(obj),
        brewersfriendkey = $form.find("input[name='brewersfriendkey']").val(),
        brewersfriendfreq = $form.find("input[name='brewersfriendfreq']").val();

    // Process post
    data = {
        brewersfriendkey: brewersfriendkey,
        brewersfriendfreq: brewersfriendfreq
    };
    postData(url, data);
}

function processBrewfatherPost(url, obj) {
    // Handle Brewfather target posts

    // Get form data
    var $form = $(obj),
        brewfatherkey = $form.find("input[name='brewfatherkey']").val(),
        brewfatherfreq = $form.find("input[name='brewfatherfreq']").val();

    // Process post
    data = {
        brewfatherkey: brewfatherkey,
        brewfatherfreq: brewfatherfreq
    };
    postData(url, data);
}

function processThingSpeakPost(url, obj) {
    // Handle ThingSpeak target posts

    // Get form data
    var $form = $(obj),
        thingspeakchannel = $form.find("input[name='thingspeakchannel']").val(),
        thingspeakkey = $form.find("input[name='thingspeakkey']").val();
    thingspeakfreq = $form.find("input[name='thingspeakfreq']").val();

    // Process post
    data = {
        thingspeakchannel: thingspeakchannel,
        thingspeakkey: thingspeakkey,
        thingspeakfreq: thingspeakfreq
    };
    postData(url, data);
}

function postData(url, data, newpage = false, newdata = false, callback = null) {
    var loadNew = (newpage.length > 0);
    $.ajax({
        url: url,
        type: 'POST',
        data: data,
        success: function(data) {
            // No alert
        },
        error: function(data) {
            alert("Settings update failed.");
        },
        complete: function(data) {
            if (loadNew) {
                window.location.href = newpage;
            } else if (newdata) {
                repopulatePage(true);
            }
            posted = true;
            if (typeof callback == "function") {
                callback();
            }
        }
    });
}