_addon.name = 'WeatherReporter'
_addon.author = 'zach2good, Heals'
_addon.version = '1.1'
_addon.command = 'WeatherReporter'

-- package.cpath somehow doesn't appreciate backslashes
local addon_path = windower.addon_path:gsub('\\', '/')
package.cpath = package.cpath .. ';' .. addon_path .. '/lib/?.dll'
require('_WeatherReporter')

windower.register_event('incoming chunk', function(id, data)
    local info = windower.ffxi.get_info()
    local player_info = windower.ffxi.get_player()
    player_info.zone = info.zone
    _WeatherReporter.submit(player_info, data)
end)
