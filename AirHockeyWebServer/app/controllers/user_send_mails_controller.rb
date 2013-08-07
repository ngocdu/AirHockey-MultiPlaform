class UserSendMailsController < ApplicationController
  # GET /user_send_mails
  # GET /user_send_mails.json
  def index
    @user_send_mails = UserSendMail.all

    respond_to do |format|
      format.html # index.html.erb
      format.json { render json: @user_send_mails }
    end
  end

  # GET /user_send_mails/1
  # GET /user_send_mails/1.json
  def show
    @user_send_mail = UserSendMail.find(params[:id])

    respond_to do |format|
      format.html # show.html.erb
      format.json { render json: @user_send_mail }
    end
  end

  # GET /user_send_mails/new
  # GET /user_send_mails/new.json
  def new
    @user_send_mail = UserSendMail.new

    respond_to do |format|
      format.html # new.html.erb
      format.json { render json: @user_send_mail }
    end
  end

  # GET /user_send_mails/1/edit
  def edit
    @user_send_mail = UserSendMail.find(params[:id])
  end

  # POST /user_send_mails
  # POST /user_send_mails.json
  def create
    @user_send_mail = UserSendMail.new(params[:user_send_mail])

    respond_to do |format|
      if @user_send_mail.save
        format.html { redirect_to @user_send_mail, notice: 'User send mail was successfully created.' }
        format.json { render json: @user_send_mail, status: :created, location: @user_send_mail }
      else
        format.html { render action: "new" }
        format.json { render json: @user_send_mail.errors, status: :unprocessable_entity }
      end
    end
  end

  # PUT /user_send_mails/1
  # PUT /user_send_mails/1.json
  def update
    @user_send_mail = UserSendMail.find(params[:id])

    respond_to do |format|
      if @user_send_mail.update_attributes(params[:user_send_mail])
        format.html { redirect_to @user_send_mail, notice: 'User send mail was successfully updated.' }
        format.json { head :no_content }
      else
        format.html { render action: "edit" }
        format.json { render json: @user_send_mail.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /user_send_mails/1
  # DELETE /user_send_mails/1.json
  def destroy
    @user_send_mail = UserSendMail.find(params[:id])
    @user_send_mail.destroy

    respond_to do |format|
      format.html { redirect_to user_send_mails_url }
      format.json { head :no_content }
    end
  end
end
