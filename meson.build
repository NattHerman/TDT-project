project('GUI Project', 'cpp',
  version : '0.1',
  default_options : ['warning_level=2',
                     'cpp_std=c++23',
                     'c_std=c17',
                     'default_library=static'])

add_project_arguments(
  '-Wno-reorder',
  '-Wno-narrowing',
  '-Wno-unused-variable',
  language : 'cpp'
)

animationwindow_dep = dependency('animationwindow', fallback: ['animationwindow', 'animationwindow_dep'])
std_lib_facilities_dep = dependency('std_lib_facilities', fallback: ['std_lib_facilities', 'std_lib_facilities_dep'])

compiler_flags = ['-Wconversion', '-fdiagnostics-color=always', '-Werror=return-type', '-static']

src = []

if host_machine.system() == 'windows'
  ld_flags = ['-static', '-lstdc++exp']
else
  ld_flags = []
endif

exe = executable(
  'program',
  src,
'Board.cpp',
'Game.cpp',
'HexagonalButton.cpp',
'UI/UINode.cpp',
'UI/HexGrid.cpp',
'UI/HexTile.cpp',
'UI/Label.cpp',
'UI/Button.cpp',
'UI/TurnIndicator.cpp',
'pathfinding/AStar.cpp',
'pathfinding/BoardNode.cpp',
'main.cpp',
'helper.cpp',
'testing.cpp',
  dependencies : [std_lib_facilities_dep, animationwindow_dep],
  cpp_args : compiler_flags,
  link_args : ld_flags
)
