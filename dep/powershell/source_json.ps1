$auth = Get-Content .\dep\conf\auth.txt
$s_o = Get-Content .\dep\conf\org_site.txt

$org = $s_o.Substring(0, $s_o.IndexOf(" "));
$site = $s_o.Substring($s_o.IndexOf(" ") + 1);

$dump = New-Item -ItemType "directory" -Name "source" -ErrorAction SilentlyContinue -InformationAction SilentlyContinue

$session = New-Object Microsoft.PowerShell.Commands.WebRequestSession
$session.UserAgent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.82 Safari/537.36"
$session.Cookies.Add((New-Object System.Net.Cookie("_ga", "GA1.3.157369595.1646051357", "/", ".nebula.zyxel.com")))
$session.Cookies.Add((New-Object System.Net.Cookie("_ga", "GA1.2.1936979852.1646052947", "/", ".zyxel.com")))
$session.Cookies.Add((New-Object System.Net.Cookie("_mkto_trk", "id:471-TTL-126&token:_mch-zyxel.com-1646052947477-14536", "/", ".zyxel.com")))
$session.Cookies.Add((New-Object System.Net.Cookie("_hjSessionUser_1413787", "eyJpZCI6IjU5Mjg0Zjk5LTI2OGUtNTYyMC1hNzM2LWE2NjBmNWQxNDMyYiIsImNyZWF0ZWQiOjE2NDYwNTI5NDc3NDYsImV4aXN0aW5nIjp0cnVlfQ==", "/", ".zyxel.com")))
$session.Cookies.Add((New-Object System.Net.Cookie("_gcl_au", "1.1.429471744.1646053838", "/", ".zyxel.com")))
$session.Cookies.Add((New-Object System.Net.Cookie("cpTouchHist_initial", "%7B%22RC_First_Channel__c%22%3A%22UnpaidEarned%22%2C%22RC_First_Medium__c%22%3A%22Referral%22%2C%22RC_First_Source_Detail__c%22%3A%22https%253A%252F%252Fselector.zyxel.com%252F%22%7D", "/", ".zyxel.com")))
$session.Cookies.Add((New-Object System.Net.Cookie("_fbp", "fb.1.1646053839308.606609328", "/", ".zyxel.com")))
$session.Cookies.Add((New-Object System.Net.Cookie("cpTouchHist_recent", "%7B%22RC_Recent_Channel__c%22%3A%22UnpaidEarned%22%2C%22RC_Recent_Medium__c%22%3A%22Referral%22%2C%22RC_Recent_Source_Detail__c%22%3A%22https%253A%252F%252Fwww.zyxel.com%252Fse%252Fsv%252Fproducts_services%252FBusiness-Firewall-USG20-VPN-USG20W-VPN%252Fdownloads%22%7D", "/", ".zyxel.com")))
$session.Cookies.Add((New-Object System.Net.Cookie("cpTouchHist_rolling", "%7B%22RC_Acquisition_Channel__c%22%3A%22UnpaidEarned%22%2C%22RC_Acquisition_Medium__c%22%3A%22Referral%22%2C%22RC_Acquisition_Source_Detail__c%22%3A%22https%253A%252F%252Fwww.zyxel.com%252Fse%252Fsv%252Fproducts_services%252FBusiness-Firewall-USG20-VPN-USG20W-VPN%252Fdownloads%22%7D", "/", ".zyxel.com")))
$session.Cookies.Add((New-Object System.Net.Cookie("__zlcmid", "190kmG8SB3UJWEJ", "/", ".zyxel.com")))
$session.Cookies.Add((New-Object System.Net.Cookie("_gid", "GA1.3.1736273765.1648447928", "/", ".nebula.zyxel.com")))
$interface = Invoke-WebRequest -UseBasicParsing -Uri "https://ccapi.nebula.zyxel.com/nebula/v9/organization/$org/site/$site/gw-flex/interface" `
-WebSession $session `
-Headers @{
  "method"="GET"
  "authority"="ccapi.nebula.zyxel.com"
  "scheme"="https"
  "path"="/nebula/v9/organization/$org/site/$site/gw-flex/interface"
  "sec-ch-ua"="`" Not A;Brand`";v=`"99`", `"Chromium`";v=`"99`", `"Google Chrome`";v=`"99`""
  "sec-ch-ua-mobile"="?0"
  "accept"="application/json, text/plain, */*"
  "x-auth-renew"="true"
  "x-refer-page"="/$org/$site/usg/configure/interface"
  "x-auth-token"="$auth"
  "sec-ch-ua-platform"="`"Windows`""
  "origin"="https://nebula.zyxel.com"
  "sec-fetch-site"="same-site"
  "sec-fetch-mode"="cors"
  "sec-fetch-dest"="empty"
  "referer"="https://nebula.zyxel.com/"
  "accept-encoding"="gzip, deflate, br"
  "accept-language"="sv-SE,sv;q=0.9,en-US;q=0.8,en;q=0.7"
}

$interface.content | Set-Content "source\interface_source.json"

$nat = Invoke-WebRequest -UseBasicParsing -Uri "https://ccapi.nebula.zyxel.com/nebula/v9/organization/$org/site/$site/gw-flex/nat" `
-WebSession $session `
-Headers @{
"method"="GET"
  "authority"="ccapi.nebula.zyxel.com"
  "scheme"="https"
  "path"="/nebula/v9/organization/$org/site/$site/gw-flex/nat"
  "sec-ch-ua"="`" Not A;Brand`";v=`"99`", `"Chromium`";v=`"99`", `"Google Chrome`";v=`"99`""
  "sec-ch-ua-mobile"="?0"
  "accept"="application/json, text/plain, */*"
  "x-auth-renew"="true"
  "x-refer-page"="/$org/$site/usg/configure/nat"
  "x-auth-token"="$auth"
  "sec-ch-ua-platform"="`"Windows`""
  "origin"="https://nebula.zyxel.com"
  "sec-fetch-site"="same-site"
  "sec-fetch-mode"="cors"
  "sec-fetch-dest"="empty"
  "referer"="https://nebula.zyxel.com/"
  "accept-encoding"="gzip, deflate, br"
  "accept-language"="sv-SE,sv;q=0.9,en-US;q=0.8,en;q=0.7"
}

$nat.content | Set-Content "source\nat_source.json"

$vpn = Invoke-WebRequest -UseBasicParsing -Uri "https://ccapi.nebula.zyxel.com/nebula/v9/organization/$org/site/$site/gw-flex/site-to-site-vpn" `
-WebSession $session `
-Headers @{
"method"="GET"
  "authority"="ccapi.nebula.zyxel.com"
  "scheme"="https"
  "path"="/nebula/v9/organization/$org/site/$site/gw-flex/site-to-site-vpn"
  "sec-ch-ua"="`" Not A;Brand`";v=`"99`", `"Chromium`";v=`"99`", `"Google Chrome`";v=`"99`""
  "sec-ch-ua-mobile"="?0"
  "accept"="application/json, text/plain, */*"
  "x-auth-renew"="true"
  "x-refer-page"="/$org/$site/usg/configure/site-to-site-vpn"
  "x-auth-token"="$auth"
  "sec-ch-ua-platform"="`"Windows`""
  "origin"="https://nebula.zyxel.com"
  "sec-fetch-site"="same-site"
  "sec-fetch-mode"="cors"
  "sec-fetch-dest"="empty"
  "referer"="https://nebula.zyxel.com/"
  "accept-encoding"="gzip, deflate, br"
  "accept-language"="sv-SE,sv;q=0.9,en-US;q=0.8,en;q=0.7"
}

$vpn.content | Set-Content "source\vpn_source.json"