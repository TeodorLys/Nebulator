$ErrorActionPreference = "silentlycontinue"
$load_token = get-content .\dep\conf\auth.txt -erroraction silentlycontinue
$dump = Invoke-WebRequest -UseBasicParsing -Uri "https://ccapi.nebula.zyxel.com/nebula/v9/group" -errorvariable ti -erroraction silentlycontinue `
-Headers @{
"method"="GET"
  "authority"="ccapi.nebula.zyxel.com"
  "scheme"="https"
  "path"="/nebula/v9/group"
  "sec-ch-ua"="`" Not A;Brand`";v=`"99`", `"Chromium`";v=`"99`", `"Google Chrome`";v=`"99`""
  "sec-ch-ua-mobile"="?0"
  "accept"="application/json, text/plain, */*"
  "x-auth-renew"="true"
  "x-refer-page"="/"
  "x-auth-token"="$load_token"
  "sec-ch-ua-platform"="`"Windows`""
  "origin"="https://nebula.zyxel.com"
  "sec-fetch-site"="same-site"
  "sec-fetch-mode"="cors"
  "sec-fetch-dest"="empty"
  "referer"="https://nebula.zyxel.com/"
  "accept-encoding"="gzip, deflate, br"
  "accept-language"="sv-SE,sv;q=0.9,en-US;q=0.8,en;q=0.7"
}

if($ti.count -eq 0){
    exit;
}

if([System.io.file]::exists(".\dep\conf\auth.txt")){
  Write-Host "Authentication token is out of date! Renewing..."
}else {
  Write-Host "Authentication for Nebula..."
}
$original_url_auth = "utf8=%E2%9C%93&authenticity_token=sbpnCFRlBaDskJoBdwrcTukfCRwh1Pd1Jw10kqFOoCBqyJpdrTUNsrmHStkEvlzy5N%2BxQMCOGOIsgMGWPDr4zw%3D%3D&user%5Bemail%5D=*EMAIL*&user%5Bpassword%5D=*PASS*&user%5Bremember_me%5D=0&user%5Bremember_me%5D=1"
Add-Type -AssemblyName System.Web

write-Host "Email: " -nonewline
$email = read-Host
$pass = Read-Host -Prompt "Password" -AsSecureString

$BSTR = [System.Runtime.InteropServices.Marshal]::SecureStringToBSTR($pass)
$upass = [System.Runtime.InteropServices.Marshal]::PtrToStringAuto($BSTR)

$upass = [uri]::EscapeDataString($upass)
$email = [uri]::EscapeDataString($email)

$original_url_auth = $original_url_auth.replace("*EMAIL*", $email)
$original_url_auth = $original_url_auth.replace("*PASS*", $upass)
$encoded_url = $original_url_auth;



$session = New-Object Microsoft.PowerShell.Commands.WebRequestSession
$session.UserAgent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.82 Safari/537.36"
$init = Invoke-WebRequest -UseBasicParsing -Uri "https://accounts.myzyxel.com/oauth2/authorize?response_type=token&client_id=d133165ec6634b380bdab3dac436159ed15c0f2de8b1148867e1c1670a7a04d8&redirect_uri=https%3A%2F%2Fnebula.zyxel.com%2Fcc%2Findex.html" `
-WebSession $session `
-Headers @{
"method"="GET"
  "authority"="accounts.myzyxel.com"
  "scheme"="https"
  "path"="/oauth2/authorize?response_type=token&client_id=d133165ec6634b380bdab3dac436159ed15c0f2de8b1148867e1c1670a7a04d8&redirect_uri=https%3A%2F%2Fnebula.zyxel.com%2Fcc%2Findex.html"
  "sec-ch-ua"="`" Not A;Brand`";v=`"99`", `"Chromium`";v=`"99`", `"Google Chrome`";v=`"99`""
  "sec-ch-ua-mobile"="?0"
  "sec-ch-ua-platform"="`"Windows`""
  "upgrade-insecure-requests"="1"
  "accept"="text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9"
  "sec-fetch-site"="cross-site"
  "sec-fetch-mode"="navigate"
  "sec-fetch-user"="?1"
  "sec-fetch-dest"="document"
  "referer"="https://nebula.zyxel.com/"
  "accept-encoding"="gzip, deflate, br"
  "accept-language"="sv-SE,sv;q=0.9,en-US;q=0.8,en;q=0.7"
}

$session.Cookies.Add($init.BaseResponse.Cookies[1])
$sign = Invoke-WebRequest -UseBasicParsing -Uri "https://accounts.myzyxel.com/users/sign_in" `
-WebSession $session `
-Headers @{
"method"="GET"
  "authority"="accounts.myzyxel.com"
  "scheme"="https"
  "path"="/users/sign_in"
  "upgrade-insecure-requests"="1"
  "accept"="text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9"
  "sec-fetch-site"="cross-site"
  "sec-fetch-mode"="navigate"
  "sec-fetch-user"="?1"
  "sec-fetch-dest"="document"
  "sec-ch-ua"="`" Not A;Brand`";v=`"99`", `"Chromium`";v=`"99`", `"Google Chrome`";v=`"99`""
  "sec-ch-ua-mobile"="?0"
  "sec-ch-ua-platform"="`"Windows`""
  "referer"="https://nebula.zyxel.com/"
  "accept-encoding"="gzip, deflate, br"
  "accept-language"="sv-SE,sv;q=0.9,en-US;q=0.8,en;q=0.7"
}


$session.Cookies.Add((New-Object System.Net.Cookie("zp_locale", "en", "/", "accounts.myzyxel.com")))
$session.Cookies.Add((New-Object System.Net.Cookie("time_zone", "Europe/Stockholm", "/", "accounts.myzyxel.com")))
$session.Cookies.Add((New-Object System.Net.Cookie("_ga", "GA1.3.60138875.1646051248", "/", ".accounts.myzyxel.com")))
$session.Cookies.Add((New-Object System.Net.Cookie("email", "nebula%40dialectkungsbacka.se", "/", "accounts.myzyxel.com")))
$session.Cookies.Add((New-Object System.Net.Cookie("_gid", "GA1.3.1846263425.1648447921", "/", ".accounts.myzyxel.com")))
#$session.Cookies = $sign.BaseResponse.Cookies[2]

$sign_auth = Invoke-WebRequest -UseBasicParsing -Uri "https://accounts.myzyxel.com/users/sign_in" `
-Method "POST" `
-WebSession $session `
-Headers @{
"method"="POST"
  "authority"="accounts.myzyxel.com"
  "scheme"="https"
  "path"="/users/sign_in"
  "cache-control"="max-age=0"
  "sec-ch-ua"="`" Not A;Brand`";v=`"99`", `"Chromium`";v=`"99`", `"Google Chrome`";v=`"99`""
  "sec-ch-ua-mobile"="?0"
  "sec-ch-ua-platform"="`"Windows`""
  "upgrade-insecure-requests"="1"
  "origin"="https://accounts.myzyxel.com"
  "accept"="text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9"
  "sec-fetch-site"="same-origin"
  "sec-fetch-mode"="navigate"
  "sec-fetch-user"="?1"
  "sec-fetch-dest"="document"
  "referer"="https://accounts.myzyxel.com/users/sign_in"
  "accept-encoding"="gzip, deflate, br"
  "accept-language"="sv-SE,sv;q=0.9,en-US;q=0.8,en;q=0.7"
} `
-ContentType "application/x-www-form-urlencoded" `
-Body $encoded_url

$oauth = Invoke-WebRequest -UseBasicParsing -Uri "https://accounts.myzyxel.com/oauth2/authorize?response_type=token&client_id=d133165ec6634b380bdab3dac436159ed15c0f2de8b1148867e1c1670a7a04d8&redirect_uri=https%3A%2F%2Fnebula.zyxel.com%2Fcc%2Findex.html" `
-WebSession $session `
-Headers @{
"method"="GET"
  "authority"="accounts.myzyxel.com"
  "scheme"="https"
  "path"="/oauth2/authorize?response_type=token&client_id=d133165ec6634b380bdab3dac436159ed15c0f2de8b1148867e1c1670a7a04d8&redirect_uri=https%3A%2F%2Fnebula.zyxel.com%2Fcc%2Findex.html"
  "cache-control"="max-age=0"
  "upgrade-insecure-requests"="1"
  "accept"="text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9"
  "sec-fetch-site"="same-origin"
  "sec-fetch-mode"="navigate"
  "sec-fetch-user"="?1"
  "sec-fetch-dest"="document"
  "sec-ch-ua"="`" Not A;Brand`";v=`"99`", `"Chromium`";v=`"99`", `"Google Chrome`";v=`"99`""
  "sec-ch-ua-mobile"="?0"
  "sec-ch-ua-platform"="`"Windows`""
  "referer"="https://accounts.myzyxel.com/users/sign_in"
  "accept-encoding"="gzip, deflate, br"
  "accept-language"="sv-SE,sv;q=0.9,en-US;q=0.8,en;q=0.7"
}


$access_token = $oauth.BaseResponse.ResponseUri.Fragment.ToString();
$access_token = $access_token.substring($access_token.indexof("=") + 1);
$access_token = $access_token.Remove($access_token.IndexOf("&"));

$trigger = Invoke-RestMethod -UseBasicParsing -Uri "https://accounts.myzyxel.com/api/v1/my?trigger=my_info&tragger_name=my_info" `
-WebSession $session `
-Headers @{
"method"="GET"
  "authority"="accounts.myzyxel.com"
  "scheme"="https"
  "path"="/api/v1/my?trigger=my_info&tragger_name=my_info"
  "sec-ch-ua"="`" Not A;Brand`";v=`"99`", `"Chromium`";v=`"99`", `"Google Chrome`";v=`"99`""
  "authorization"="Bearer " + $access_token
  "sec-ch-ua-mobile"="?0"
  "sec-ch-ua-platform"="`"Windows`""
  "accept"="*/*"
  "origin"="https://nebula.zyxel.com"
  "sec-fetch-site"="cross-site"
  "sec-fetch-mode"="cors"
  "sec-fetch-dest"="empty"
  "referer"="https://nebula.zyxel.com/"
  "accept-encoding"="gzip, deflate, br"
  "accept-language"="sv-SE,sv;q=0.9,en-US;q=0.8,en;q=0.7"
  "if-none-match"="W/`"007646600ea635e08de6f7e985fd0edb`""
}

$wan_auth = Invoke-WebRequest -UseBasicParsing -Uri "https://sd-wan.nebula.zyxel.com/nebula/v3/account/auth" `
-Method "OPTIONS" `
-WebSession $session `
-Headers @{
"Accept"="*/*"
  "Access-Control-Request-Method"="POST"
  "Access-Control-Request-Headers"="content-type"
  "Origin"="https://nebula.zyxel.com"
  "Sec-Fetch-Mode"="cors"
  "Sec-Fetch-Site"="same-site"
  "Sec-Fetch-Dest"="empty"
  "Referer"="https://nebula.zyxel.com/"
  "Accept-Encoding"="gzip, deflate, br"
  "Accept-Language"="sv-SE,sv;q=0.9,en-US;q=0.8,en;q=0.7"
}

$session.Cookies.Add($wan_auth.BaseResponse.Cookies);
$body = "{`"user_info`":`"" + $trigger.result + "`",`"zyxel_oauth_token`":`"" + $access_token + "`"}"
$auth_wan = Invoke-WebRequest -UseBasicParsing -Uri "https://sd-wan.nebula.zyxel.com/nebula/v3/account/auth" `
-Method "POST" `
-WebSession $session `
-Headers @{
"sec-ch-ua"="`" Not A;Brand`";v=`"99`", `"Chromium`";v=`"99`", `"Google Chrome`";v=`"99`""
  "Accept"="application/json, text/javascript, */*; q=0.01"
  "sec-ch-ua-mobile"="?0"
  "sec-ch-ua-platform"="`"Windows`""
  "Origin"="https://nebula.zyxel.com"
  "Sec-Fetch-Site"="same-site"
  "Sec-Fetch-Mode"="cors"
  "Sec-Fetch-Dest"="empty"
  "Referer"="https://nebula.zyxel.com/"
  "Accept-Encoding"="gzip, deflate, br"
  "Accept-Language"="sv-SE,sv;q=0.9,en-US;q=0.8,en;q=0.7"
} `
-ContentType "application/json; charset=UTF-8" `
-Body $body

$f_body = "{`"user_info`":`"" + $trigger.result + "`"}";

$final = Invoke-RestMethod -UseBasicParsing -Uri "https://ccapi.nebula.zyxel.com/nebula/v3/account/auth" `
-Method "POST" `
-WebSession $session `
-Headers @{
"method"="POST"
  "authority"="ccapi.nebula.zyxel.com"
  "scheme"="https"
  "path"="/nebula/v3/account/auth"
  "sec-ch-ua"="`" Not A;Brand`";v=`"99`", `"Chromium`";v=`"99`", `"Google Chrome`";v=`"99`""
  "accept"="application/json, text/javascript, */*; q=0.01"
  "sec-ch-ua-mobile"="?0"
  "sec-ch-ua-platform"="`"Windows`""
  "origin"="https://nebula.zyxel.com"
  "sec-fetch-site"="same-site"
  "sec-fetch-mode"="cors"
  "sec-fetch-dest"="empty"
  "referer"="https://nebula.zyxel.com/"
  "accept-encoding"="gzip, deflate, br"
  "accept-language"="sv-SE,sv;q=0.9,en-US;q=0.8,en;q=0.7"
} `
-ContentType "application/json; charset=UTF-8" `
-Body $f_body

$final.body.authtoken | set-content .\dep\conf\auth.txt
