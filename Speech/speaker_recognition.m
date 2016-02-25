%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clc;
clear all;
close all;
MFCC_size=12;
GMMM_component= 50;

mu_model=zeros(MFCC_size,GMMM_component);
sigma_model=zeros(MFCC_size,GMMM_component);
weight_model=zeros(GMMM_component);

train_file_path='./1/1-';
num_train=6;
test_file_path='./';
num_test=15;
num_uttenance=3;

all_train_feature=[];
%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%train model
for i=1:num_train
    train_file=[train_file_path num2str(i) 'normal.wav'];
    [wav_data ,fs]=audioread(train_file);
    train_feature=melcepst(wav_data ,fs);  
    all_train_feature=[all_train_feature;train_feature];
    
end
[mu_model,sigma_model,weight_model]=gmm_estimate(all_train_feature',GMMM_component);

%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%test
totalscore = [];
for i=1:num_test
    for j=1:num_uttenance
        score = [];
        for k = 1:6
            if i==1 && j==1
                continue
            end
            if j==1
                speed = 'normal';
            elseif j==2
                speed = 'slow';
            else
                speed = 'fast';
            end
            test_file=[test_file_path num2str(i) '/' num2str(i) '-' num2str(k) speed '.wav'];
            [wav_data ,fs]=audioread(test_file);
            test_feature=melcepst(wav_data ,fs);
            [lYM, lY] = lmultigauss(test_feature', mu_model, sigma_model, weight_model);
            score(k) = mean(lY);
            fprintf('Test:%d_%d_%s  score:%f\n',i,k,speed,score(k));
        end  
        totalscore = [totalscore;score];

    end
end

%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%
%result
[max_score,max_id]=max(totalscore);
max_score = max(max_score);
[min_score,min_id]=min(totalscore);
min_score = min(min_score);

totalscore = (totalscore - min_score)./(max_score-min_score);
fprintf('Max score:%f\nMin score:%f\n',max_score,min_score);
% fprintf('Max id:%f Min id:%f\n',max_id,min_id);