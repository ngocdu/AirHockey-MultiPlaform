class User3sController < ApplicationController
  # GET /user3s
  # GET /user3s.json
  def index
    @user3s = User3.all

    respond_to do |format|
      format.html # index.html.erb
      format.json { render json: @user3s }
    end
  end

  # GET /user3s/1
  # GET /user3s/1.json
  def show
    if params[:name] != nil
      n = params[:id]
      @user = User3.find_by_name(n)
      respond_to do |format|
        format.html # show.html.erb
        format.json { render json: @user }
      end
    end
    
    if params[:email] != nil
      e = params[:email]
      @user = User3.find_by_email(e)
      respond_to do |format|
        format.html # show.html.erb
        format.json { render json: @user }
      end
    end
  end

  # GET /user3s/new
  # GET /user3s/new.json
  def new
    @user3 = User3.new

    respond_to do |format|
      format.html # new.html.erb
      format.json { render json: @user3 }
    end
  end

  # GET /user3s/1/edit
  def edit
    @user3 = User3.find(params[:id])
  end

  # POST /user3s
  # POST /user3s.json
  def create
    @user = User3.new(name: params[:name], point: params[:point], email: params[:email], reward: 0)
      respond_to do |format|
        if @user.save
          format.html { redirect_to @users, notice: 'User was successfully created.' }
          format.json { render json: @users, status: :created, location: @user }
        else
          format.html { render action: "new" }
          format.json { render json: @user.errors, status: :unprocessable_entity }
        end
      end
      #-----------------------------------------------------------------------------
  end

  # PUT /user3s/1
  # PUT /user3s/1.json
  def update
    @user3 = User3.find(params[:id])

    respond_to do |format|
      if @user3.update_attributes(params[:user3])
        format.html { redirect_to @user3, notice: 'User3 was successfully updated.' }
        format.json { head :no_content }
      else
        format.html { render action: "edit" }
        format.json { render json: @user3.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /user3s/1
  # DELETE /user3s/1.json
  def destroy
    @user3 = User3.find(params[:id])
    @user3.destroy

    respond_to do |format|
      format.html { redirect_to user3s_url }
      format.json { head :no_content }
    end
  end
end
