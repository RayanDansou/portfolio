from rest_framework import serializers
from .models import BlogPost,Book,Review,Emotion

class BlogPostserialiser(serializers.ModelSerializer):
    class Meta:
        model=BlogPost
        fields= [ "id","title","content","date" ] # champs qui seront retounés dans les reponses de l'API
        
        
        
class EmotionSerializer(serializers.ModelSerializer):
    class Meta:
        model=Emotion
        fields= [ "id","nom_emotion" ]     
    
        
class BookSerializer(serializers.ModelSerializer):
    class Meta:
        model=Book
        fields= [ "id","title","author","description","genre","publication_date","emotions" ]     
        
   

class ReviewSerializer(serializers.ModelSerializer):

    class Meta:
        model = Review
        fields = ['book', 'rating', 'comment', 'review_date'] #le field book renverra un objet book représenté dans la table review
        
