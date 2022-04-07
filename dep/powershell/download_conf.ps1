add-type @"
    using System.Net;
    using System.Security.Cryptography.X509Certificates;
    public class TrustAllCertsPolicy : ICertificatePolicy {
        public bool CheckValidationResult(
            ServicePoint srvPoint, X509Certificate certificate,
            WebRequest request, int certificateProblem) {
            return true;
        }
    }
"@
[System.Net.ServicePointManager]::CertificatePolicy = New-Object TrustAllCertsPolicy

$ErrorActionPreference = "silentlycontinue"

"nope" | set-content ".\dep\conf\startup-config.conf"

write-Host "IP: " -nonewline
$ip = read-host
if($ip -eq $null){
  write-host "Failed"
  "nope" | set-content ".\dep\conf\startup-config.conf"
  exit;
}

$dump = test-connection $ip -count 1 -ErrorAction SilentlyContinue -ErrorVariable tt
if($tt.count -gt 0){
  write-host "Failed"
  "nope" | set-content ".\dep\conf\startup-config.conf"
  exit;
}
write-host "OK"
write-Host "USER: " -nonewline
$email = read-Host
$pass = Read-Host -Prompt "PASSWORD " -AsSecureString

$BSTR = [System.Runtime.InteropServices.Marshal]::SecureStringToBSTR($pass)
$upass = [System.Runtime.InteropServices.Marshal]::PtrToStringAuto($BSTR)

$original_url_auth = "username=*USER*&pwd=*PASS*&pwd_r=&password=*PASS*"

$original_url_auth = $original_url_auth.replace("*USER*", $email)
$original_url_auth = $original_url_auth.replace("*PASS*", $upass)


$session = New-Object Microsoft.PowerShell.Commands.WebRequestSession
$session.UserAgent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.82 Safari/537.36"
$raw = Invoke-WebRequest -UseBasicParsing -Uri "https://$ip/" -ErrorAction SilentlyContinue -ErrorVariable init -TimeoutSec 2 `
-WebSession $session `
-Headers @{
"sec-ch-ua"="`" Not A;Brand`";v=`"99`", `"Chromium`";v=`"99`", `"Google Chrome`";v=`"99`""
  "sec-ch-ua-mobile"="?0"
  "sec-ch-ua-platform"="`"Windows`""
  "Upgrade-Insecure-Requests"="1"
  "Accept"="text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9"
  "Sec-Fetch-Site"="none"
  "Sec-Fetch-Mode"="navigate"
  "Sec-Fetch-User"="?1"
  "Sec-Fetch-Dest"="document"
  "Accept-Encoding"="gzip, deflate, br"
  "Accept-Language"="sv-SE,sv;q=0.9"
}
if($init.count -gt 0){
  write-host $init
    "nope" | set-content ".\dep\conf\startup-config.conf"
    exit
}

$login = Invoke-WebRequest -UseBasicParsing -Uri "https://$ip/" -ErrorAction SilentlyContinue -ErrorVariable sec_init -TimeoutSec 2 `
-Method "POST" `
-WebSession $session `
-Headers @{
"Cache-Control"="max-age=0"
  "sec-ch-ua"="`" Not A;Brand`";v=`"99`", `"Chromium`";v=`"99`", `"Google Chrome`";v=`"99`""
  "sec-ch-ua-mobile"="?0"
  "sec-ch-ua-platform"="`"Windows`""
  "Upgrade-Insecure-Requests"="1"
  "Origin"="https://$ip"
  "Accept"="text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9"
  "Sec-Fetch-Site"="same-origin"
  "Sec-Fetch-Mode"="navigate"
  "Sec-Fetch-User"="?1"
  "Sec-Fetch-Dest"="document"
  "Referer"="https://$ip/"
  "Accept-Encoding"="gzip, deflate, br"
  "Accept-Language"="sv-SE,sv;q=0.9"
} `
-ContentType "application/x-www-form-urlencoded" `
-Body $original_url_auth
if($sec_init.count -gt 0){
  write-host $sec_init;
    "nope" | set-content ".\dep\conf\startup-config.conf"
    exit
}

$license = Invoke-WebRequest -UseBasicParsing -Uri "https://$ip/ext-js/app/view/pagestore/license_reminding.html" -ErrorAction SilentlyContinue -ErrorVariable lic -TimeoutSec 2 `
-WebSession $session `
-Headers @{
"Cache-Control"="max-age=0"
  "Upgrade-Insecure-Requests"="1"
  "Accept"="text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9"
  "Sec-Fetch-Site"="same-origin"
  "Sec-Fetch-Mode"="navigate"
  "Sec-Fetch-User"="?1"
  "Sec-Fetch-Dest"="document"
  "sec-ch-ua"="`" Not A;Brand`";v=`"99`", `"Chromium`";v=`"99`", `"Google Chrome`";v=`"99`""
  "sec-ch-ua-mobile"="?0"
  "sec-ch-ua-platform"="`"Windows`""
  "Referer"="https://$ip/"
  "Accept-Encoding"="gzip, deflate, br"
  "Accept-Language"="sv-SE,sv;q=0.9"
}
if($lic.count -gt 0){
  write-host $lic
    "nope" | set-content ".\dep\conf\startup-config.conf"
    exit
}


$res = Invoke-WebRequest -UseBasicParsing -Uri "https://$ip/cgi-bin/export-cgi?category=config&arg0=startup-config.conf" -ErrorAction SilentlyContinue -ErrorVariable conf -TimeoutSec 2 `
-WebSession $session `
-Headers @{
"sec-ch-ua"="`" Not A;Brand`";v=`"99`", `"Chromium`";v=`"99`", `"Google Chrome`";v=`"99`""
  "sec-ch-ua-mobile"="?0"
  "sec-ch-ua-platform"="`"Windows`""
  "Upgrade-Insecure-Requests"="1"
  "Accept"="text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9"
  "Sec-Fetch-Site"="same-origin"
  "Sec-Fetch-Mode"="navigate"
  "Sec-Fetch-User"="?1"
  "Sec-Fetch-Dest"="document"
  "Referer"="https://$ip/ext-js/index.html"
  "Accept-Encoding"="gzip, deflate, br"
  "Accept-Language"="sv-SE,sv;q=0.9"
}
if($conf.count -gt 0){
  write-host $conf
    "nope" | set-content ".\dep\conf\startup-config.conf"
    exit
}

if($res.Content[0] -ne '!'){
     "nope" | set-content ".\dep\conf\startup-config.conf"
      exit;
}

$res.content | set-content ".\dep\conf\startup-config.conf"