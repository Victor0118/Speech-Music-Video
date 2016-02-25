clc;clear;close all;
MFCC_size=12;%mfcc???
GMM_component=16;%GMM component ??
mu_model=zeros(MFCC_size,GMM_component);%???? ?? ??
sigma_model=zeros(MFCC_size,GMM_component);%???? ?? ??
weight_model=zeros(GMM_component);%???? ?? ??
train_file_path='1/1-';%???????
num_train=6;%????
test_file_path='./';%?????? 2~15?
num_test=15;%????,?15?
num_uttenance=6;%???? ????6*3

all_train_feature =[];
all_scores = [];

for i=1:num_train
    train_file=[train_file_path num2str(i) 'normal' '.wav']; %?????
    [wav_data ,fs]=readwav(train_file); %??????
    train_feature=melcepst(wav_data ,fs); %????
    all_train_feature=[all_train_feature;train_feature]; %?????????
end

[mu_model,sigma_model,weight_model]=gmm_estimate(all_train_feature',GMM_component); %?????????
%test
for i=1:num_test %?????;?15??????????????????
    for j=1:num_uttenance %?????
        for k=1:3   %??normal, fast, slow
            test_file=[num2str(i) '/' num2str(i) '-' num2str(j)]; %?????
            if(k==1) str = 'normal.wav'; end
            if(k==2) str = 'fast.wav'; end
            if(k==3) str = 'slow.wav'; end   
            test_file = strcat(test_file,str);
            [wav_data ,fs]=readwav(test_file); %??????
            test_feature=melcepst(wav_data ,fs); %????
            [lYM, lY] = lmultigauss(test_feature', mu_model,sigma_model, weight_model); %???????
            score(i) = mean(lY); %????
            all_scores(i,j,k) = score(i); %??????
            fprintf('Test:%d-%d%s score:%f\n',i,j,str,score(i)); %??
        end
    end
end

max1 = max(max(all_scores(:,:,1)));
max2 = max(max(all_scores(:,:,2)));
max3 = max(max(all_scores(:,:,3)));

min1 = min(min(min(all_scores)));
