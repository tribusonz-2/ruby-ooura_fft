# frozen_string_literal: true
require 'mkmf'

have_header('windows.h')
have_header('pthread.h')

have_func('pthread_create', 'pthread.h')
have_func('pthread_join', 'pthread.h')
have_func('CreateThread', 'windows.h')
have_func('WaitForSingleObject', 'windows.h')
have_func('CloseHandle', 'windows.h')

have_header('stdbit.h')
have_func('ispow2l', 'stdbit.h')

$INSTALLFILES = Dir.glob(%w[ooura_fft/*.h]).map{|x| [x,'$(archdir)'] }

create_makefile('ooura_fft/ooura_fft')
