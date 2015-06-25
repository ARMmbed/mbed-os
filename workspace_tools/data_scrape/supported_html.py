HEADER = """
<!DOCTYPE html>
<html>
<head>
<style>

table, th, td {
    border: 1px solid black;
     border-collapse: collapse;
}

.rotate {
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
<button class="sort" data-sort="target">
  Sort by target
  </button>
<table id="example" class="dataTable display" cellspacing="0" width="80%">
"""

AVAILABLE = ["NO", "YES"]
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
"""

SCRIPT3 = """
</script>
"""

FILTER1 = """
$('#filter-"""


FILTER2 = """').click(function() {
  featureList.filter(function(item) {
    if (item.values()."""

FILTER3 = """ == "YES") {
      return true;
    } else {
      return false;
    }
  });
  return false;
});
"""
