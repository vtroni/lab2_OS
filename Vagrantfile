Vagrant.configure("2") do |config|
  config.vm.box = "bento/ubuntu-26.04"

  config.vm.provider "virtualbox" do |vb|
    vb.memory = 2048
    vb.cpus = 2
  end

  config.vm.provision "shell", inline: <<~SHELL
    export DEBIAN_FRONTEND=noninteractive
    apt-get update
    apt-get install -y build-essential strace util-linux time manpages-dev
  SHELL
end
