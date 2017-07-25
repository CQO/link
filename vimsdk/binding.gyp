{
    'targets':[
        {
            'target_name':'nodeService',
            'cflags': [ '-std=c++11', '-Wignored-attributes', '-Wmissing-braces'], #'-Wall',

            'sources':[
                'src/nodeService.cpp',
				'src/client.cpp',
				'src/sdkChannel.cpp',
				'src/service/authService.cpp',
				'src/service/chatService.cpp',
				'src/service/contactService.cpp',
				'src/service/configService.cpp',
				'src/service/groupService.cpp',
				'src/service/fileService.cpp',
				'src/service/userService.cpp',
				'src/service/searchService.cpp',
				'src/utils/helper.cpp',
				'src/utils/utils.cpp'
            ],
			'link_settings': {
				'libraries': [
				'C:/imsdkapi.lib'
				]
			},
            'conditions': [
            [ 'OS=="mac"', {

            'xcode_settings': {
                'OTHER_CPLUSPLUSFLAGS' : ['-stdlib=libc++'],
                },

            }],
            ],
        }
    ]
}
