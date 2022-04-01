$policy = get-executionpolicy
$policy | set-content .\dep\conf\policy.txt -erroraction silentlycontinue
if($policy -eq "Bypass"){
    exit;
}

$currentPrincipal = New-Object Security.Principal.WindowsPrincipal([Security.Principal.WindowsIdentity]::GetCurrent())
if($currentPrincipal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator) -eq $false){
    Write-host "ExecutionPolicy was set to" -nonewline -foregroundcolor blue
    write-host " $policy " -nonewline -foregroundcolor red
    write-host "this program requires bypass to run properly" -foregroundcolor blue
    write-host "Restart the program as administrator and we will change the policy to" -nonewline  -foregroundcolor blue
    write-host " Bypass " -foregroundcolor green
    write-host "then revert back!" -foregroundcolor blue
    write-host "Alternatively change the policy yourself and just restart the program" -foregroundcolor blue
    Write-host "set-ExecutionPolicy -Scope CurrentUser Bypass -force" -foregroundcolor green
    "" | set-content .\dep\conf\nope.txt
    exit;
}

if($args[0] -eq "-revert"){
    $rev_policy = get-content .\dep\conf\policy.txt
    set-executionpolicy -Scope CurrentUser $rev_policy -force 
}

if($policy -ne "Bypass"){
    set-executionpolicy -Scope CurrentUser Bypass -force
}

