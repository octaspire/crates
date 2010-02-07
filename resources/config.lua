config = {}
config.verbose                                 = 0       -- [0..2]
config.filelog                                 = false
config.fps                                     = 25
config.fullscreen                              = true

--[[
Please note that if  you  enable  sounds  and  your  systems  has
PulseAudio,  the SDL(_mixer) + PulseAudio combination might cause
problems at least on some  GNU/Linux‐systems.   See  for  example
"the  pulseaudio  hack".  For  example, at the time of writing in
Ubuntu, if your system has SDL with ALSA option (libsdl1.2debian‐
alsa)  installed,  sounds  might  be crackling, channels will ebb
away and game will _GET JAMMED_ when you try to quit  playing. In
this case removing libsdl1.2debian‐alsa  and installing SDL  with
PulseAudio options (libsdl1.2debian‐pulseaudio) instead helped.
--]]
config.sounds                                  = false
config.music                                   = false

config.mix_channels                            = 16
config.music_volume                            = 0.5     -- [0..1]
config.sound_volume                            = 0.5     -- [0..1]
config.sound_stereo                            = false
config.sound_chunksize                         = 256
config.controllers                             = true
config.controller_nth_to_use                   = 1
config.controller_ok                           = 1
config.controller_exit                         = 9
config.controller_restart                      = 10
config.controller_hat_up                       = 1
config.controller_hat_down                     = 4
config.controller_hat_left                     = 8
config.controller_hat_right                    = 2
config.controller_axis_up_down_1               = 1
config.controller_axis_up_down_2               = 3
config.controller_axis_right_left_1            = 0
config.controller_axis_right_left_2            = 2
config.controller_axis_trigger_down_right_min  = 32000
config.controller_axis_trigger_down_right_max  = 32769
config.controller_axis_trigger_up_left_min     = -32769
config.controller_axis_trigger_up_left_max     = -32000
config.controller_button_repeat_delay          = 30      -- NOT milliseconds. Instead number of frames waited before repeat is activated.
config.controller_button_repeat_interval       = 8       -- NOT milliseconds, Instead number of frames betweeen repeated events.
config.grabinputfullscreen                     = false
config.backfaceculling                         = true
config.lighting                                = true
config.vertical_synchronization                = true

config.skin                                    = "waymark" -- Texture theme to be used: "waymark" or "cartoon"

config.screen_width                          = 640
config.screen_height                         = 480
--config.screen_bpp                          = 0
