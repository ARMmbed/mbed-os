HEADER = """
<!DOCTYPE html>
<html>
<head>
<style>

td {
    border: 1px solid black;
     border-collapse: collapse;
     font-weight: bold;
}

table {
    border: 1px solid white;
     border-collapse: collapse;
}

.rotate {
    font-weight: 900;
    height: 140px;
    white-space: nowrap;
    width: 30px;
    position: relative;
    top: 75px;
     right: -50px;
     -moz-transform: rotate(-45.0deg);  /* FF3.5+ */
      -ms-transform: rotate(-45.0deg);  /* IE9+ */
       -o-transform: rotate(-45.0deg);  /* Opera 10.5 */
  -webkit-transform: rotate(-45.0deg);  /* Safari 3.1+, Chrome */
          transform: rotate(-45.0deg);  /* Standard */

}
</style>
</head>
<body>
<div id="users">
<input class="search" placeholder="Search" />
<table id="example" class="dataTable display" cellspacing="0" width="80%">
"""

AVAILABLE = ["<span style=\"color:#DF013A\"> NO </span>", "<span style=\"color:#31B404\"> YES </span>"]
SROW = "<tr>\n"
EROW = "</tr>\n"
SBODY = "       <tbody class=\"list\">\n"
EBODY = "</tbody>\n"
SHEAD = "<thead>\n"
EHEAD = "</thead>\n"
SHEADER1 = "<th><div class=\"rotate\" id=\"filter-"
SHEADER2 = "\">"
EHEADER = "</div></th>\n"
SCELL1 = "          <td class = \""
SCELL2 = "\">"
ECELL = "</td>\n"

FOOTER = """</table>
</div>
</body>
"""

SCRIPT1="""
<script src="https://developer.mbed.org/media/uploads/Sissors/list.js"></script>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/2.1.4/jquery.min.js"></script>
<script>
var options = {
  valueNames: ["""

SCRIPT2 = """]
};

var featureList = new List('users', options);

var stateArray = new Array(options.valueNames.length);

function runFilter() {
    featureList.filter(function(item) {
    for (var i = 0; i<options.valueNames.length; i++) {
        if (stateArray[i] != undefined) {
            if (item.values()[options.valueNames[i]].indexOf(stateArray[i]) == -1) {
                return false;
            }
        }
    }
    return true;
  });
  return false;
};
"""

SCRIPT3 = """
</script>
"""

FILTER1 = """
$('#filter-"""


FILTER2 = """').click(function() {
  var loc = options.valueNames.indexOf(\""""

FILTER3 = """\");
    if (stateArray[loc] == "YES") {
        stateArray[loc] = undefined;
        $(this).css({
        'text-decoration': 'none'
    });
    } else {
        stateArray[loc] = "YES";
        $(this).css({
        'text-decoration': 'underline'
    });
    }
    
    runFilter();
})
"""