cmake_minimum_required(VERSION 3.22.1)

project(pbdoom)

find_package(Inkview REQUIRED)
include_directories(${INKVIEW_INCLUDE_DIR})

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIR_freetype2})

### Fill version.h.in with versions https://jonathanhamberg.com/post/cmake-embedding-git-hash/
# Get tag info
execute_process(
  COMMAND git describe --tags
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_TAG
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

configure_file(
  ${CMAKE_SOURCE_DIR}/version.h.in
  ${CMAKE_BINARY_DIR}/generated/version.h
)
include_directories(${CMAKE_BINARY_DIR}/generated)
### end version


add_executable(game.app
  main.c
  pbdoom_state.h
  pbdoom_state.c
  events.h
  events.c
  pbdoom/render.h
  pbdoom/render.c
  pbdoom/frames/game.h
  pbdoom/frames/game.c
  pbdoom/frames/keyboard.h
  pbdoom/frames/keyboard.c
  pbdoom/frames/panel.h
  pbdoom/frames/panel.c


sdldoom-1.10/am_map.c
sdldoom-1.10/am_map.h
sdldoom-1.10/d_englsh.h
sdldoom-1.10/d_event.h
sdldoom-1.10/d_french.h
sdldoom-1.10/d_items.c
sdldoom-1.10/d_items.h
sdldoom-1.10/d_main.c
sdldoom-1.10/d_main.h
sdldoom-1.10/d_net.c
sdldoom-1.10/d_net.h
sdldoom-1.10/doomdata.h
sdldoom-1.10/doomdef.c
sdldoom-1.10/doomdef.h
sdldoom-1.10/doomstat.c
sdldoom-1.10/doomstat.h
sdldoom-1.10/doomtype.h
sdldoom-1.10/d_player.h
sdldoom-1.10/dstrings.c
sdldoom-1.10/dstrings.h
sdldoom-1.10/d_textur.h
sdldoom-1.10/d_think.h
sdldoom-1.10/d_ticcmd.h
sdldoom-1.10/f_finale.c
sdldoom-1.10/f_finale.h
sdldoom-1.10/f_wipe.c
sdldoom-1.10/f_wipe.h
sdldoom-1.10/g_game.c
sdldoom-1.10/g_game.h
sdldoom-1.10/hu_lib.c
sdldoom-1.10/hu_lib.h
sdldoom-1.10/hu_stuff.c
sdldoom-1.10/hu_stuff.h
sdldoom-1.10/i_main.c
sdldoom-1.10/i_main.h
sdldoom-1.10/i_net.c
sdldoom-1.10/i_net.h
sdldoom-1.10/info.c
sdldoom-1.10/info.h
sdldoom-1.10/install-sh
sdldoom-1.10/i_sound.c
sdldoom-1.10/i_sound.h
sdldoom-1.10/i_system.c
sdldoom-1.10/i_system.h
sdldoom-1.10/i_video.c
sdldoom-1.10/i_video.h
sdldoom-1.10/m_argv.c
sdldoom-1.10/m_argv.h
sdldoom-1.10/m_bbox.c
sdldoom-1.10/m_bbox.h
sdldoom-1.10/m_cheat.c
sdldoom-1.10/m_cheat.h
sdldoom-1.10/m_fixed.c
sdldoom-1.10/m_fixed.h
sdldoom-1.10/m_menu.c
sdldoom-1.10/m_menu.h
sdldoom-1.10/m_misc.c
sdldoom-1.10/m_misc.h
sdldoom-1.10/m_random.c
sdldoom-1.10/m_random.h
sdldoom-1.10/m_swap.c
sdldoom-1.10/m_swap.h
sdldoom-1.10/p_ceilng.c
sdldoom-1.10/p_doors.c
sdldoom-1.10/p_enemy.c
sdldoom-1.10/p_floor.c
sdldoom-1.10/p_inter.c
sdldoom-1.10/p_inter.h
sdldoom-1.10/p_lights.c
sdldoom-1.10/p_local.h
sdldoom-1.10/p_map.c
sdldoom-1.10/p_maputl.c
sdldoom-1.10/p_mobj.c
sdldoom-1.10/p_mobj.h
sdldoom-1.10/p_plats.c
sdldoom-1.10/p_pspr.c
sdldoom-1.10/p_pspr.h
sdldoom-1.10/p_saveg.c
sdldoom-1.10/p_saveg.h
sdldoom-1.10/p_setup.c
sdldoom-1.10/p_setup.h
sdldoom-1.10/p_sight.c
sdldoom-1.10/p_spec.c
sdldoom-1.10/p_spec.h
sdldoom-1.10/p_switch.c
sdldoom-1.10/p_telept.c
sdldoom-1.10/p_tick.c
sdldoom-1.10/p_tick.h
sdldoom-1.10/p_user.c
sdldoom-1.10/r_bsp.c
sdldoom-1.10/r_bsp.h
sdldoom-1.10/r_data.c
sdldoom-1.10/r_data.h
sdldoom-1.10/r_defs.h
sdldoom-1.10/r_draw.c
sdldoom-1.10/r_draw.h
sdldoom-1.10/r_local.h
sdldoom-1.10/r_main.c
sdldoom-1.10/r_main.h
sdldoom-1.10/r_plane.c
sdldoom-1.10/r_plane.h
sdldoom-1.10/r_segs.c
sdldoom-1.10/r_segs.h
sdldoom-1.10/r_sky.c
sdldoom-1.10/r_sky.h
sdldoom-1.10/r_state.h
sdldoom-1.10/r_things.c
sdldoom-1.10/r_things.h
sdldoom-1.10/sounds.c
sdldoom-1.10/sounds.h
sdldoom-1.10/s_sound.c
sdldoom-1.10/s_sound.h
sdldoom-1.10/st_lib.c
sdldoom-1.10/st_lib.h
sdldoom-1.10/st_stuff.c
sdldoom-1.10/st_stuff.h
sdldoom-1.10/tables.c
sdldoom-1.10/tables.h
sdldoom-1.10/v_video.c
sdldoom-1.10/v_video.h
sdldoom-1.10/wi_stuff.c
sdldoom-1.10/wi_stuff.h
sdldoom-1.10/w_wad.c
sdldoom-1.10/w_wad.h
sdldoom-1.10/z_zone.c
sdldoom-1.10/z_zone.h

)
target_link_libraries(game.app ${INKVIEW_LIBRARIES})
target_link_libraries(game.app m)

target_compile_options(game.app PRIVATE -Werror=implicit-function-declaration)
