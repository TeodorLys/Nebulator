local script
if arg[1] == "-send" then
    script = [[
        powershell ".\dep\powershell\script.ps1"
    ]]
elseif arg[1] == "-auth" then
    script = [[
        powershell ".\dep\powershell\nebula_auth.ps1"
    ]]
elseif arg[1] == "-conf" then
    script = [[
        powershell ".\dep\powershell\download_conf.ps1"
    ]]
elseif arg[1] == "-sel_org" then
    script = [[
        powershell ".\dep\powershell\sel_org_site.ps1"
    ]]
elseif arg[1] == "-json" then
    script = [[
        powershell ".\dep\powershell\source_json.ps1"
    ]]
elseif arg[1] == "-exec" then
    script = [[
        powershell "get-executionpolicy | set-content .\dep\conf\policy.txt; set-executionpolicy -Scope CurrentUser bypass -force"
    ]]
elseif arg[1] == "-exec_revert" then
    script = [[
        powershell "invoke-command -scriptblock{$policy = get-content .\dep\conf\policy.txt; set-executionpolicy -scope currentuser $policy;}"
    ]]
end
os.execute(script)
