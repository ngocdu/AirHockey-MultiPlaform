class UsersController < ApplicationController
  # GET /users
  # GET /users.json
  def index
    @users = User.all
    respond_to do |format|
      format.html
      format.json { render json: @users }
    end

  end

  # GET /users/1
  # GET /users/1.json
  def show
    @user = User.find(params[:id])
    respond_to do |format|
      format.html # show.html.erb
      format.json { render json: @user }
    end
    # binding.pry
    #  n = params[:id]
    # @user = User.find_by_name(n)
    # respond_to do |format|
    #   format.html # show.html.erb
    #   format.json { render json: @user }
    # end
  end

  # GET /users/new
  # GET /users/new.json
  def new
    @user = User.new

    respond_to do |format|
      format.html # new.html.erb
      format.json { render json: @user }
    end
  end

  # GET /users/1/edit
  def edit
    @user = User.find(params[:id])
  end

  # POST /users
  # POST /users.json
  def create
    point_new = params[:point]
    ti = params[:time]
    if params[:reward] != nil
        User.all.each do |user|
            if user.updated_at.strftime("%Y-%m-%dT%H:%M:%SZ") == ti &&
                user.name == params[:name] && user.email == params[:email] &&
                user.reward != 0
                user.destroy
                @user = User.new(name: params[:name], point: params[:point],
                                 email: params[:email], reward: -1)
                @user.save
                @u = UserSendMail.new(name: params[:name], email: params[:email])
                @u.save
                return
            end
        end 
    end
    if User.all.count >= 10
      point_min = User.last.point
      point_max = User.first.point
      user_min = User.last
      user_max = User.first
      if User.find_by_email(params[:email]) != nil
        
      end
      if point_max < point_new.to_i
        user_min.destroy
        @user = nil
          @user = User.new(name: params[:name], point: params[:point],
            email: params[:email], reward: 1)
        respond_to do |format|
          if @user.save
            User3.create!(user_id: @user.id, point: params[:point])
            format.html { redirect_to @users, notice: 'User was successfully created.' }
            format.json { render json: @users, status: :created, location: @user }
          else
            format.html { render action: "new" }
            format.json { render json: @user.errors, status: :unprocessable_entity }
          end
        end
      else
          user_min.destroy
          @user = nil
          @user = User.new(name: params[:name], point: params[:point],
                           email: params[:email], reward: 0)
          @user.save
          User3.create!(user_id: @user.id, point: params[:point])
      end
    else
      @user = User.new(name: params[:name], point: params[:point], email: params[:email], reward: 1)
        respond_to do |format|
          if @user.save
            User3.create!(user_id: @user.id, point: params[:point])
            format.html { redirect_to @users, notice: 'User was successfully created.' }
            format.json { render json: @users, status: :created, location: @user }
          else
            format.html { render action: "new" }
            format.json { render json: @user.errors, status: :unprocessable_entity }
          end
        end
     end
  end
  
  def updateEmail
    
  end
  # PUT /users/1
  # PUT /users/1.json
  def update
    @user = User.find(params[:id])
    respond_to do |format|
      if @user.update_attributes(params[:user])
        format.html { redirect_to @user, notice: 'User was successfully updated.' }
        format.json { head :no_content }
      else
        format.html { render action: "edit" }
        format.json { render json: @user.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /users/1
  # DELETE /users/1.json
  def destroy
    @user = User.find(params[:id])
    @user.destroy

    respond_to do |format|
      format.html { redirect_to users_url }
      format.json { head :no_content }
    end
  end
end