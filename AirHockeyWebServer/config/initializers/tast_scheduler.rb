require 'rubygems'; require 'rufus/scheduler'

scheduler = Rufus::Scheduler.start_new
scheduler.cron '0 36 11 * * 1-7' do
    UserSendMail.all.each do |u|
        UserMailer.welcome_email(u).deliver
        u.destroy
    end
 end