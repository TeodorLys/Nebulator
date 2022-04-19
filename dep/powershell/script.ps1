$body = Get-Content -raw -Path 'source\vpn.json'
$url = 'https://ccapi.nebula.zyxel.com/nebula/v9/organization/623acc482e139e19872ea7b9/site/623acc6e6426b7e745f755b9/gw-flex/site-to-site-vpn'
$session = New-Object Microsoft.PowerShell.Commands.WebRequestSession;
$session.UserAgent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.74 Safari/537.36";
$ErrorActionPreference = "silentlycontinue"
$raw = Invoke-WebRequest -UseBasicParsing -Uri $url -ErrorAction SilentlyContinue -ErrorVariable e `
-Method "PUT" `
-WebSession $session `
 -Headers @{
 "method"="PUT"
 "authority"="ccapi.nebula.zyxel.com" 
"scheme"="https" 
"path"="/nebula/v9/organization/623acc482e139e19872ea7b9/site/623acc6e6426b7e745f755b9/gw-flex/site-to-site-vpn" 
"sec-ch-ua"="`" Not A;Brand`";v=`"99`", `"Chromium`";v=`"99`", `"Google Chrome`";v=`"99`"" 
"sec-ch-ua-mobile"="?0" 
"accept"="application/json, text/plain, */*"
 "x-auth-renew"="true" 
"x-refer-page"="/623acc482e139e19872ea7b9/623acc6e6426b7e745f755b9/usg/configure/site-to-site-vpn" 
"x-auth-token"="0fe99433daaa478894a9bb892f634d33" 
"sec-ch-ua-platform"="`"Windows`"" 
"origin"="https://nebula.zyxel.com"
 "sec-fetch-site"="same-site" 
"sec-fetch-mode"="cors" 
"sec-fetch-dest"="empty"
 "referer"="https://nebula.zyxel.com/"
"accept-encoding"="gzip, deflate, br" 
"accept-language"="sv-SE,sv;q=0.9,en-US;q=0.8,en;q=0.7" 
} `
-ContentType "application/json" `
-Body $body;
$raw.statusdescription
foreach($a in $e){
"[POWERSHELL]: " + $a.message.tostring() | add-content "log0.txt"
}