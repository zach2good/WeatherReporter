_addon.name = 'WeatherReporter'
_addon.author = 'zach2good, Heals'
_addon.version = '1.0'
_addon.command = 'WeatherReporter'

-- package.cpath somehow doesn't appreciate backslashes
local addon_path = windower.addon_path:gsub('\\', '/')
package.cpath = package.cpath .. ';' .. addon_path .. '/lib/?.dll'
require('_WeatherReporter')

windower.register_event('incoming chunk', function(id, data)
    if id == 0x00A then -- Zone In
        _WeatherReporter.submit(windower.ffxi.get_info(), data)
    elseif id == 0x057 then -- Weather Change
        _WeatherReporter.submit(windower.ffxi.get_info(), data)
    end
end)
