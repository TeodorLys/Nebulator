$load_token = get-content .\dep\conf\auth.txt

$session = New-Object Microsoft.PowerShell.Commands.WebRequestSession
$session.UserAgent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.82 Safari/537.36"
$raw = Invoke-RestMethod -UseBasicParsing -Uri "https://ccapi.nebula.zyxel.com/nebula/v9/organization" `
-Method "POST" `
-WebSession $session `
-Headers @{
"method"="POST"
  "authority"="ccapi.nebula.zyxel.com"
  "scheme"="https"
  "path"="/nebula/v9/organization"
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
} `
-ContentType "application/json" `
-Body "{}"

for($a = 0; $a -lt $raw.body.Count; $a++){
    write-host "ID:{"$a "}" $raw.body[$a].organization_name
}
$orgok = $false;
while($orgok -eq $false){
    Write-Host "Select organization by typing id"
    Write-Host "> " -NoNewline
    $in = Read-Host

    if($in -gt $raw.body.Count){
        Write-Host "Out of range!"
    }elseif($in -lt $raw.body.Count){
        $orgok = $true;
    }
}

$_org = $raw.body[$in];
$uri = "https://ccapi.nebula.zyxel.com/nebula/v9/organization/" + $_org.organization_id + "/sites"

$sites = Invoke-RestMethod -UseBasicParsing -Uri $uri `
-Method "POST" `
-WebSession $session `
-Headers @{
"method"="POST"
  "authority"="ccapi.nebula.zyxel.com"
  "scheme"="https"
  "path"="/nebula/v9/organization/" + $_org.organization_id + "/sites"
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
} `
-ContentType "application/json" `
-Body "{`"privilege_required`":false}"

if($sites.body.sites.Count -eq 1){
    Write-Host "Organization:" $_org.organization_name "Site: " $sites.body.sites[0].site_name
    $_site = $sites.body.sites[0];
}else{
    for($a = 0; $a -lt $raw.body.Count; $a++){
    write-host "ID:" $a $sites.body.sites[$a]
    }
    $orgok = $false;
    while($orgok -eq $false){
        Write-Host "Select site by typing id"
        Write-Host "> " -NoNewline
        $in = Read-Host

        if($in -gt $sites.body.Count){
            Write-Host "Out of range!"
        }elseif($in -lt $sites.body.Count){
            $orgok = $true;
        }
    }

    $_site = $sites.body.sites[$in];
}
$final = $_org.organization_id + " " + $_site.site_id
$final | set-content ".\dep\conf\org_site.txt"