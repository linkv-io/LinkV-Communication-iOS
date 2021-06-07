
Pod::Spec.new do |s|
  s.name             = 'LinkV-Communication'
  s.version          = '0.0.2'
  s.summary          = 'RTC + IM 优化接口'

  s.description      = <<-DESC
 RTC + IM you can send im message
 update api
                       DESC

  s.homepage         = 'https://github.com/linkv-io/LinkV-Communication-iOS'
  s.license          = { :type => 'MIT', :file => 'LICENSE' }
  s.author           = { 'wangyansnow' => 'wangyan6@linkv.sg' }
  s.source           = { :git => 'https://github.com/linkv-io/LinkV-Communication-iOS.git', :tag => s.version.to_s }

  s.ios.deployment_target = '9.0'
  s.source_files = 'LVCEngine/LVCEngine/LinkV_RTC/**/*'
  s.public_header_files = 'LVCEngine/LVCEngine/LinkV_RTC/**/*.h'
  # 需要引入的自己打包的.framework
  s.vendored_frameworks = 'LVCEngine/framework/LinkV.framework', 'LVCEngine/framework/LVIMLib.framework'
  # 需要引入的自己打包的.a静态库文件
  s.vendored_libraries = 'LVCEngine/framework/libcares.a', 'LVCEngine/framework/libopencore-amrnb.a', 'LVCEngine/framework/libprotobuf-lite.a'
  # 需要引用的资源
  s.resource = 'LVCEngine/resources/default.metallib'
  # 依赖的系统框架
  s.libraries = 'c++', 'resolv.9', 'sqlite3', 'z'
    
  s.xcconfig = {'ENABLE_BITCODE' => 'NO'}
end
