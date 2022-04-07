local script = [[
    powershell ".\dep\powershell\sel_org_site.ps1"
    
]]

--local pip = io.popen("powershell -command -", "w")
os.execute(script)
--pip:write(script)
--pip:close()