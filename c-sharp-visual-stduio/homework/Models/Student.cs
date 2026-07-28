using System;

namespace Homework.Models
{
    public class Student
    {
        public float _score { get; private set; } = 0;
        public string _grade { get; private set; } = "";

        private const string gradeA = "Grade [A] Excellent";
        private const string gradeB = "Grade [B] Grate";
        private const string gradeC = "Grade [C] Very Good";
        private const string gradeD = "Grade [D] Good";
        private const string gradeE = "Grade [E] Fair";
        private const string gradeF = "Grade [F] Poor";

        public string CalculateGrade(float score)
        {
            this._score = score;

            if (score >= 90) { this._grade = gradeA; }
            else if (score >= 80) { this._grade = gradeB; }
            else if (score >= 70) { this._grade = gradeC; }
            else if (score >= 60) { this._grade = gradeD; }
            else if (score >= 50) { this._grade = gradeE; }
            else { this._grade = gradeF; }

            return this._grade;
        }
    }
}